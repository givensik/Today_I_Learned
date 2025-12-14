#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <unistd.h>
#include <fcntl.h>
#include <cjson/cJSON.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define GENERATE_API_URL "http://localhost:11434/api/generate" // 단순 질의용 API URL
#define CHAT_API_URL "http://localhost:11434/api/chat" // 컨텍스트 기반 대화용 API URL
#define MODEL_NAME_QWEN3 "qwen3:0.6b" // Qwen3, r 모델이라 좀 느림
#define MODEL_NAME_GEMMA3_270M "gemma3:270m" // Gemma3, 270m 밖에 안해서 빠름
#define MODEL_NAME_GEMMA3_1B "gemma3:1b" // Gemma3 1B
#define MODEL_NAME_GPT_OSS "gpt-oss:20b" // gpu 좋은 것 있으신 분은 gpt-oss:20b 혹은 gpt-oss:120b 써보세요.

#define MY_PORT 5555 // 미니쉘 서버 포트

// libcurl4, cJSON 설치
// sudo apt-get install libcurl4-openssl-dev libcjson-dev
// 컴파일시 라이브러리 링크 옵션 추가 필요:
// gcc -o test_ai_helper test_ai_helper.c -lcurl -lcjson
// 혹은 ./vscode/tasks.json 에서 링크 옵션 추가(args: ["-lcurl", "-lcjson"])

// ---------------------------------------------------------
// 함수 원형 선언
// ---------------------------------------------------------

// 스트림 콜백 기본 구현 
static void socket_stream_cb(const char *chunk, void *user) {
    int cfd = *(int *)user;            // 클라이언트 소켓 FD
    if (!chunk || cfd <= 0) return;
    size_t len = strlen(chunk);
    // write가 블로킹일 수 있으므로 반복해서 모두 전송
    ssize_t written = 0;
    const char *p = chunk;
    while (len > 0) {
        ssize_t w = write(cfd, p, len);
        if (w < 0) {
            // 에러 발생 시 중단
            perror("socket_stream_cb write");
            break;
        }
        written += w;
        p += w;
        len -= w;
    }
}
typedef void (*ai_stream_cb)(const char *chunk, void *user);

int ai_chat_with_context(char *log_mem, size_t *log_size, const char *user_input,
                         char *assistant_output, size_t out_size, const char *model);
int ai_chat_with_context_stream(char *log_mem, size_t *log_size, const char *user_input,
                                char *assistant_output, size_t out_size, const char *model,
                                ai_stream_cb cb, void *cb_user);
static void append_log(char *context_mem, size_t *cm_size, const char *role, const char *text);
static cJSON* build_messages_json(char *context_mem, size_t cm_size);
static int call_chat_api(cJSON *messages, const char *model, char *out, size_t out_sz,
                         ai_stream_cb cb, void *cb_user);

// 파일 기반 컨텍스트 저장 (기본 1MiB)
#define AI_LOG_CAPACITY (1024 * 1024)
static int ai_ctx_fd = -1;
static char *ai_ctx_map = NULL;
static size_t ai_ctx_size = 0;
static size_t ai_ctx_capacity = AI_LOG_CAPACITY;

// 초기화: 파일을 생성(또는 열고)하고 용량을 설정 후 비우기(truncate)
int init_ai_context_file(const char *path) {
    if (!path) return -1;
    int fd = open(path, O_RDWR | O_CREAT, 0644);
    if (fd < 0) return -1;
    if (ftruncate(fd, (off_t)ai_ctx_capacity) != 0) {
        close(fd);
        return -1;
    }
    void *m = mmap(NULL, ai_ctx_capacity, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (m == MAP_FAILED) {
        close(fd);
        return -1;
    }
    // 초기화: 빈 파일로 취급 (사이즈 0)
    ai_ctx_fd = fd;
    ai_ctx_map = (char *)m;
    ai_ctx_size = 0;
    ai_ctx_map[0] = '\0';
    return 0;
}

// 종료: 맵 해제 및 파일 닫기
void close_ai_context_file(void) {
    if (ai_ctx_map) {
        msync(ai_ctx_map, ai_ctx_capacity, MS_SYNC);
        munmap(ai_ctx_map, ai_ctx_capacity);
        ai_ctx_map = NULL;
    }
    if (ai_ctx_fd >= 0) {
        close(ai_ctx_fd);
        ai_ctx_fd = -1;
    }
    ai_ctx_size = 0;
}

// libcurl 응답 저장용 구조체
typedef struct {
    char *buf;           // 누적 버퍼 (개행 단위 파싱 위해 유지)
    size_t len;
    char *out;           // 최종 응답 버퍼
    size_t out_sz;
    size_t out_len;
    int error;
    void (*stream_cb)(const char *chunk, void *user);
    void *cb_user;
} ChatStreamCtx;

// ---------------------------------------------------------
// REPL main: 한 줄 입력 → 응답 한 줄 출력 (stdout)
// ---------------------------------------------------------
int main(void) {

    const char *MODEL = MODEL_NAME_GEMMA3_1B;

    // ---------- 1) TCP 서버 소켓 생성 ----------
    // your code here
    int sfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sfd < 0) {
        perror("socket");
        exit(1);
    }

    int opt = 1;
    // 주소 재사용 옵션 설정. 다음 시간에 배울꺼라, 여기서는 알아만 두는 걸로. 
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 서버 주소 구조체, 클라이언트 주소 구조체 선언
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    // 클라이언트 주소 길이 변수 선언 및 초기화
    // your code here
    socklen_t cli_len = sizeof(cli_addr);
    // memset을 이용한 구조체 초기화
    // your code here
    memset(&serv_addr, 0, sizeof(serv_addr));
    // 주소 체계: IPv4 설정
    // your code here
    serv_addr.sin_family = AF_INET;
    // 모든 인터페이스에서 수신
    // your code here
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    // 포트 번호 설정
    // your code here
    serv_addr.sin_port = htons(MY_PORT);

    // 소켓을 IP/포트에 바인딩. 서버만 bind 필요
    if (bind(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    // 클라이언트 접속을 받을 준비 (블로킹 아님). 최대 8개 대기 큐
    if (listen(sfd, 8) < 0) {
        perror("listen");
        exit(1);
    }

    printf("[AI Helper] Listening on port %d...\n", MY_PORT);
    fflush(stdout);

    // ---------- 2) 초기 컨텍스트 파일 ----------
    init_ai_context_file("prompt.log");

    // ---------- 3) accept 루프 ----------
    while (1) {
        // accept : 클라이언트 접속 요청을 수락
        // your code here
        int cfd = accept(sfd, (struct sockaddr *)&cli_addr, &cli_len);
        if (cfd < 0) {
            perror("accept");
            continue;
        }

        printf("[AI Helper] Client connected\n");
        fflush(stdout);

        char prompt[4096];
        char response[8192];

        // ---------- 4) client socket 기반 REPL ----------
        while (1) {

            // 클라이언트로부터 프롬프트 수신. read() 사용
            // your code here
            ssize_t n = read(cfd, prompt, sizeof(prompt)-1);
            if (n <= 0) break;

            prompt[n] = '\0';

            // mini-shell이 newline 포함해서 보냄: 제거
            size_t len = strlen(prompt);
            if (len && prompt[len-1]=='\n')
                prompt[len-1]='\0';

            if (prompt[0]=='\0') continue;
            
            printf("Received prompt: %s\n", prompt);
            fflush(stdout);
            // ---------- 5) Gemma3/Qwen3 호출 (기존과 동일) ----------
            int ret = ai_chat_with_context_stream(
                NULL,
                NULL,
                prompt,
                response,
                sizeof(response),
                MODEL,
                // streaming callback → socket으로 전송
                // stdout으로 프린트하던 내용을 socket으로
                // 그대로 전달하면 됨 
                socket_stream_cb,
                &cfd
            );

            if (ret != 0) {
                const char *err = "[AI ERROR]";
                // 소켓으로 에러 메시지 전송
                write(cfd, err, strlen(err));
            }

            // ---------- 6) mini-shell 종료 마커 전송 ----------
            // your code here
            const char *end_marker = "\n<<<END>>>\n";
            write(cfd, end_marker, strlen(end_marker));
        }

        printf("[AI Helper] Client disconnected\n");
        // ---------- 7) 클라이언트 소켓 종료 ----------
        // your code here
        close(cfd);
    }

    close_ai_context_file();
    // 서버 소켓 종료
    // your code here
    close(sfd);
    return 0;
}



// libcurl 콜백 함수
static size_t stream_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    ChatStreamCtx *ctx = (ChatStreamCtx *)userp;
    if (ctx->error) return realsize;

    // 누적 버퍼 확장
    char *ptr = realloc(ctx->buf, ctx->len + realsize + 1);
    if (!ptr) { ctx->error = 1; return 0; }
    ctx->buf = ptr;
    memcpy(ctx->buf + ctx->len, contents, realsize);
    ctx->len += realsize;
    ctx->buf[ctx->len] = '\0';

    // 개행 단위로 파싱
    size_t start = 0;
    for (size_t i = 0; i < ctx->len; ++i) {
        if (ctx->buf[i] == '\n') {
            size_t line_len = i - start;
            if (line_len > 0) {
                char saved = ctx->buf[start + line_len];
                ctx->buf[start + line_len] = '\0';
                cJSON *parsed = cJSON_Parse(ctx->buf + start);
                ctx->buf[start + line_len] = saved;

                if (parsed) {
                    cJSON *msg = cJSON_GetObjectItem(parsed, "message");
                    cJSON *content = msg ? cJSON_GetObjectItem(msg, "content") : NULL;
                    if (content && cJSON_IsString(content) && content->valuestring) {
                        size_t add_len = strlen(content->valuestring);
                        if (ctx->out_len + add_len + 1 < ctx->out_sz) {
                            memcpy(ctx->out + ctx->out_len, content->valuestring, add_len);
                            ctx->out_len += add_len;
                            ctx->out[ctx->out_len] = '\0';
                        }
                        if (ctx->stream_cb) {
                            ctx->stream_cb(content->valuestring, ctx->cb_user);
                        }
                    }
                    cJSON_Delete(parsed);
                }
            }
            start = i + 1;
        }
    }

    // 남은 부분을 앞으로 당겨 보존
    if (start > 0 && start < ctx->len) {
        memmove(ctx->buf, ctx->buf + start, ctx->len - start);
        ctx->len -= start;
        ctx->buf[ctx->len] = '\0';
    } else if (start == ctx->len) {
        ctx->len = 0;
    }

    return realsize;
}

// 메모리에 직접 로그 추가
static void append_log(char *context_mem, size_t *cm_size, const char *role, const char *text) {
    // If explicit memory buffer provided, use it (preserve existing behavior)
    if (context_mem && cm_size) {
        size_t max_write = 1024 * 1024 - *cm_size; // 남은 공간 계산
        if (max_write < 100) return; // 공간 부족 시 중단
        int len = snprintf(context_mem + *cm_size, max_write, "%s: %s\n", role, text);
        if (len > 0 && (size_t)len < max_write) {
            *cm_size += len;
        }
        return;
    }

    // Otherwise, if file-backed context is initialized, append there
    if (ai_ctx_map) {
        char tmp[4096];
        int len = snprintf(tmp, sizeof(tmp), "%s: %s\n", role, text);
        if (len <= 0) return;
        size_t need = (size_t)len;

        // If not enough space, trim oldest lines until it fits
        while (ai_ctx_size + need >= ai_ctx_capacity) {
            // find first newline
            char *p = memchr(ai_ctx_map, '\n', ai_ctx_size);
            if (!p) {
                // no newline -> reset buffer
                ai_ctx_size = 0;
                ai_ctx_map[0] = '\0';
                break;
            }
            size_t remove = (p - ai_ctx_map) + 1;
            memmove(ai_ctx_map, ai_ctx_map + remove, ai_ctx_size - remove);
            ai_ctx_size -= remove;
            ai_ctx_map[ai_ctx_size] = '\0';
        }

        if (ai_ctx_size + need < ai_ctx_capacity) {
            memcpy(ai_ctx_map + ai_ctx_size, tmp, need);
            ai_ctx_size += need;
            ai_ctx_map[ai_ctx_size] = '\0';
            // persist to file mapping
            msync(ai_ctx_map, ai_ctx_capacity, MS_SYNC);
        }
    }
}

// 메모리에서 직접 읽어서 cJSON 배열 구성
static cJSON* build_messages_json(char *context_mem, size_t cm_size) {
    if (cm_size == 0) {
        return cJSON_CreateArray();
    }

    cJSON *messages = cJSON_CreateArray();
    char *ptr = context_mem;
    char *end = context_mem + cm_size;

    // 메모리에서 직접 파싱 (디스크 I/O 최소화)
    while (ptr < end) {
        char *line_end = memchr(ptr, '\n', end - ptr); // 한 줄의 끝 찾기, memchr : 메모리 블록에서 특정 문자 찾기
        if (!line_end) line_end = end;

        size_t line_len = line_end - ptr;
        
        // USER: 파싱
        if (line_len > 6 && strncmp(ptr, "USER: ", 6) == 0) {
            char content[4096];
            size_t content_len = line_len - 6;
            if (content_len >= sizeof(content)) content_len = sizeof(content) - 1;
            // USER: 이후 내용 복사, memcpy 사용 
            memcpy(content, ptr + 6, content_len); 
            content[content_len] = '\0';

            cJSON *msg = cJSON_CreateObject();
            cJSON_AddStringToObject(msg, "role", "user");
            cJSON_AddStringToObject(msg, "content", content);
            cJSON_AddItemToArray(messages, msg);
        }
        // ASSISTANT: 파싱
        else if (line_len > 11 && strncmp(ptr, "ASSISTANT: ", 11) == 0) {
            char content[4096];
            size_t content_len = line_len - 11;
            if (content_len >= sizeof(content)) content_len = sizeof(content) - 1;
            // ASSISTANT: 이후 내용 복사. memcpy 사용
            memcpy(content, ptr + 11, content_len); 
            content[content_len] = '\0';

            cJSON *msg = cJSON_CreateObject();
            cJSON_AddStringToObject(msg, "role", "assistant");
            cJSON_AddStringToObject(msg, "content", content);
            cJSON_AddItemToArray(messages, msg);
        }

        ptr = line_end + 1;
    }

    return messages;
}

// /api/chat 호출 (cJSON 사용)
static int call_chat_api(cJSON *messages, const char *model, char *out, size_t out_sz,
                         ai_stream_cb stream_cb, void *cb_user) {
    if (out_sz == 0) return -1;
    out[0] = '\0';

    CURL *curl = curl_easy_init();
    if (!curl) return -1;

    // JSON payload 생성
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "model", model);
    cJSON_AddItemToObject(root, "messages", messages);
    cJSON_AddBoolToObject(root, "stream", 1);
    
    char *json_payload = cJSON_PrintUnformatted(root);
    
    ChatStreamCtx ctx = {0};
    ctx.out = out;
    ctx.out_sz = out_sz;
    ctx.out_len = 0;
    ctx.stream_cb = stream_cb;
    ctx.cb_user = cb_user;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    const char *api_url = getenv("AI_HELPER_API");
    if (!api_url) api_url = CHAT_API_URL;
    curl_easy_setopt(curl, CURLOPT_URL, api_url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
    // 디버그 출력: 필요하면 컴파일 시 `-DAI_HELPER_DEBUG` 추가
#ifdef AI_HELPER_DEBUG
    fprintf(stderr, "[ai_helper_chat] JSON payload: %s\n", json_payload);
#endif
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, stream_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ctx);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30L); // 연결 5초
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);       // 전체 15초

        CURLcode res = curl_easy_perform(curl);
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    #ifdef AI_HELPER_DEBUG
        fprintf(stderr, "[ai_helper_chat] curl_perform res=%d (%s), http_code=%ld\n",
                (int)res, curl_easy_strerror(res), http_code);
    #endif
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(json_payload);
    cJSON_Delete(root);

    // 남은 버퍼에 개행 없이 도착한 마지막 JSON 처리
    if (ctx.len > 0 && !ctx.error) {
        ctx.buf = realloc(ctx.buf, ctx.len + 1);
        if (ctx.buf) {
            ctx.buf[ctx.len] = '\0';
            cJSON *parsed = cJSON_Parse(ctx.buf);
            if (parsed) {
                cJSON *msg = cJSON_GetObjectItem(parsed, "message");
                cJSON *content = msg ? cJSON_GetObjectItem(msg, "content") : NULL;
                if (content && cJSON_IsString(content) && content->valuestring) {
                    size_t add_len = strlen(content->valuestring);
                    if (ctx.out_len + add_len + 1 < ctx.out_sz) {
                        memcpy(ctx.out + ctx.out_len, content->valuestring, add_len);
                        ctx.out_len += add_len;
                        ctx.out[ctx.out_len] = '\0';
                    }
                    if (ctx.stream_cb) {
                        ctx.stream_cb(content->valuestring, ctx.cb_user);
                    }
                }
                cJSON_Delete(parsed);
            }
        }
    }

    free(ctx.buf);

    if (res != CURLE_OK || ctx.error || ctx.out_len == 0) {
#ifdef AI_HELPER_DEBUG
        fprintf(stderr, "[ai_helper_chat] curl_res=%d (%s), ctx.error=%d, ctx.out_len=%zu\n",
                (int)res, curl_easy_strerror(res), ctx.error, ctx.out_len);
#endif
        return -1;
    }

    out[ctx.out_len < out_sz ? ctx.out_len : out_sz - 1] = '\0';
    return 0;
}

// 컨텍스트 기반 AI 대화 (메모리 주소 기반)
int ai_chat_with_context(char *context_mem, size_t *cm_size, const char *user_input,
                         char *assistant_output, size_t out_size, const char *model) {
    return ai_chat_with_context_stream(context_mem, cm_size, user_input,
                                       assistant_output, out_size, model,
                                       NULL, NULL);
}

// 스트리밍 콜백 지원 버전
int ai_chat_with_context_stream(char *context_mem, size_t *cm_size, const char *user_input,
                                char *assistant_output, size_t out_size, const char *model,
                                ai_stream_cb cb, void *cb_user) {
    // Determine which context buffer to use: explicit memory or file-backed
    char *used_mem = NULL;
    size_t used_sz = 0;
    size_t *used_sz_ptr = NULL;
    if (context_mem && cm_size) {
        used_mem = context_mem;
        used_sz = *cm_size;
        used_sz_ptr = cm_size;
    } else if (ai_ctx_map) {
        used_mem = ai_ctx_map;
        used_sz = ai_ctx_size;
        used_sz_ptr = &ai_ctx_size;
    }

    // 1) USER 입력 로그에 추가 (메모리 또는 파일에 기록)
    append_log(used_mem, used_sz_ptr, "USER", user_input);

    // 2) 메모리에서 직접 읽어서 messages JSON 생성
    size_t current_sz = used_sz_ptr ? *used_sz_ptr : used_sz;
    cJSON *messages_json = build_messages_json(used_mem, current_sz);
    if (!messages_json) return -1;

    // 3) /api/chat 호출 (call_chat_api가 messages_json의 소유권을 가져감)
    int ret = call_chat_api(messages_json, model, assistant_output, out_size, cb, cb_user);
    printf("AI Response: %s\n", assistant_output); // 디버그용 출력
    // ret 가 0이 아니면 에러
    if (ret != 0) return -1;

    // 4) ASSISTANT 응답 로그에 추가 (메모리 또는 파일에 기록)
    append_log(used_mem, used_sz_ptr, "ASSISTANT", assistant_output);

    return 0;
}
