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
#include <poll.h>
#include <errno.h>
#include <time.h>

#define GENERATE_API_URL "http://localhost:11434/api/generate"
#define CHAT_API_URL "http://localhost:11434/api/chat"
#define MODEL_NAME_QWEN3 "qwen3:0.6b"
#define MODEL_NAME_GEMMA3_270M "gemma3:270m"
#define MODEL_NAME_GEMMA3_1B "gemma3:1b"
#define MODEL_NAME_GPT_OSS "gpt-oss:20b"

#define MY_PORT 5555
#define MAX_CLIENTS 1024

#define AI_LOG_CAPACITY (1024 * 1024)

typedef void (*ai_stream_cb)(const char *chunk, void *user);

typedef struct {
    char *buf;
    size_t len;
    char *out;
    size_t out_sz;
    size_t out_len;
    int error;
    ai_stream_cb stream_cb;
    void *cb_user;
} ChatStreamCtx;

// 클라이언트 컨텍스트 구조체
typedef struct {
    int   fd;       // 클라이언트 소켓 파일 디스크립터
    int   ctx_fd;   // 컨텍스트 로그 파일 디스크립터
    char *ctx_map;  // 컨텍스트 로그 메모리 맵
    size_t ctx_size; // 컨텍스트 로그 현재 크기
} ClientCtx;

// 클라이언트 컨텍스트 배열
static ClientCtx *clients[MAX_CLIENTS];

// forward declarations
static void socket_stream_cb(const char *chunk, void *user);
static void append_log(char *context_mem, size_t *cm_size, const char *role, const char *text);
static cJSON* build_messages_json(char *context_mem, size_t cm_size);
static int call_chat_api(cJSON *messages, const char *model, char *out,
                         size_t out_sz, ai_stream_cb cb, void *cb_user);
int ai_chat_with_context(char *context_mem, size_t *cm_size, const char *user_input,
                         char *assistant_output, size_t out_size, const char *model);
int ai_chat_with_context_stream(char *context_mem, size_t *cm_size, const char *user_input,
                                char *assistant_output, size_t out_size, const char *model,
                                ai_stream_cb cb, void *cb_user);

// ----------------------------------------------------------------------
// 스트림 콜백: chunk → 클라이언트 소켓 즉시 전송
// ----------------------------------------------------------------------
static void socket_stream_cb(const char *chunk, void *user) {
    int cfd = *(int *)user;
    write(cfd, chunk, strlen(chunk));
}

// ----------------------------------------------------------------------
// main(): poll 기반 멀티클라이언트 AI 헬퍼 서버
// ----------------------------------------------------------------------
int main(void)
{
    const char *MODEL = MODEL_NAME_GEMMA3_1B;

    // 서버 소켓 생성
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) { perror("socket"); exit(1); }

    int opt = 1;
    // 주소 재사용 옵션 설정. 바로 다시 시작 가능하도록 
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr, cli;
    socklen_t cli_len = sizeof(cli);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; // IPv4
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 인터페이스에서 수신
    addr.sin_port = htons(MY_PORT); // 포트 번호 설정

    // 서버는 bind() 호출
    if (bind(sfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) { 
        perror("bind");
        exit(1);
    }

    // 100명 동시 접속 대기
    if (// your code here) {
        perror("listen");
        exit(1);
    }

    printf("[AI Helper] Listening on port %d...\n", MY_PORT);

    // ------------------------------------------------------------------
    // 멀티 유저를 위한 poll 구조 설정
    // pollfds[0] = listening socket, sfd
    // pollfds[i] = client socket or -1 (unused) // cfd
    // ------------------------------------------------------------------
    // your code here
    // 모든 poll fd 초기화
    struct pollfd pollfds[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++) {
        pollfds[i].fd = -1;
        pollfds[i].events = POLLIN;
    }


    // listening socket, sfd 설정 
    pollfds[0].fd = sfd;
    pollfds[0].events = POLLIN;

    char prompt[4096];
    char response[8192];

    while (1) {

        // poll() 호출. 블로킹 모드. 
        // your code here
        if (nready < 0) {
            if (errno == EINTR) continue;
            perror("poll");
            break;
        }

        // --------------------------------------------------------------
        // 1) listening socket ready? → accept()
        // 만약 listening socket이 준비되었으면, 새로운 클라이언트 접속 수락
        // --------------------------------------------------------------        
        if (// your code here) {

            int cfd = accept(sfd, (struct sockaddr *)&cli, &cli_len); // 접속 허락 
            if (cfd < 0) {
                perror("accept");
            } else {

                // 새로운 슬롯 탐색
                int idx = -1;
                for (int i = 1; i < MAX_CLIENTS; i++) {
                    if ( // your code here) {
                        idx = i;
                        break; // 빈 슬롯 발견
                    }
                }

                if (idx == -1) { // 빈 슬롯이 없다면, -1일 테니까. 
                    close(cfd); // 최대 이용자수가 넘쳐서 수용 불가
                } else {
                    // 새로운 클라이언트 소켓을 pollfd에 등록
                    // your code here
                    // your code here

                    ClientCtx *ctx = calloc(1, sizeof(ClientCtx));
                    ctx->fd = cfd;

                    time_t now = time(NULL);
                    char fname[256];
                    snprintf(fname, sizeof(fname),
                             "prompt_session_%ld_%d.log", (long)now, cfd);

                    // 대화를 유지할 프롬프트 로그 파일 생성
                    int logfd = open(fname, O_RDWR | O_CREAT | O_TRUNC, 0644);
                    ftruncate(logfd, AI_LOG_CAPACITY);

                    void *m = mmap(NULL, AI_LOG_CAPACITY,
                                   PROT_READ | PROT_WRITE, MAP_SHARED,
                                   logfd, 0);

                    ctx->ctx_fd = logfd;
                    ctx->ctx_map = (char *)m;
                    ctx->ctx_size = 0;

                    clients[cfd] = ctx;

                    printf("[AI Helper] Client connected (fd=%d, file=%s)\n",
                           cfd, fname);
                }
            }
            if (--nready <= 0) continue;
        }

        // --------------------------------------------------------------
        // 2) client socket ready? → read()
        // --------------------------------------------------------------
        for (int i = 1; i < MAX_CLIENTS; i++) {

            int fd = pollfds[i].fd;
            if (fd < 0) continue;
            
            // i번째 클라이언트 소켓이 준비되었으면, 처리 
            if (// your code here) {

                ClientCtx *ctx = clients[fd];
                if (!ctx) {
                    char trash[1024];
                    read(fd, trash, sizeof(trash));
                    continue;
                }  
                // 클라이언트로부터 프롬프트 읽기
                ssize_t n = read(fd, prompt, sizeof(prompt) - 1);
                // 연결이 종료되면 EOF 반환 
                if (n <= 0) {
                    printf("[AI Helper] Client disconnected (fd=%d)\n", fd);
                    munmap(ctx->ctx_map, AI_LOG_CAPACITY);
                    close(ctx->ctx_fd);
                    free(ctx);
                    clients[fd] = NULL;

                    close(fd);
                    pollfds[i].fd = -1;

                    continue;
                }

                prompt[n] = '\0';
                size_t len = strlen(prompt);
                if (len && prompt[len - 1] == '\n')
                    prompt[len - 1] = '\0';

                if (prompt[0] == '\0') continue;

                int ret = ai_chat_with_context_stream(
                    ctx->ctx_map,
                    &ctx->ctx_size,
                    prompt,
                    response,
                    sizeof(response),
                    MODEL,
                    socket_stream_cb,
                    &fd
                );

                if (ret != 0) {
                    const char *e = "[AI ERROR]";
                    write(fd, e, strlen(e));
                }

                write(fd, "<<<END>>>", 9);

                if (--nready <= 0) break;
            }
        }
    }

    // 다 끝나면, 서버 소켓 닫기
    close(sfd);
    return 0;
}

// ----------------------------------------------------------------------
// 스트림 콜백: CURL write callback
// ----------------------------------------------------------------------

static size_t stream_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    ChatStreamCtx *ctx = (ChatStreamCtx *)userp;
    if (ctx->error) return realsize;

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

                    if (content && cJSON_IsString(content)) {
                        size_t add_len = strlen(content->valuestring);

                        if (ctx->out_len + add_len + 1 < ctx->out_sz) {
                            memcpy(ctx->out + ctx->out_len,
                                   content->valuestring, add_len);
                            ctx->out_len += add_len;
                            ctx->out[ctx->out_len] = '\0';
                        }
                        if (ctx->stream_cb)
                            ctx->stream_cb(content->valuestring, ctx->cb_user);
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

static void append_log(char *context_mem, size_t *cm_size,
                       const char *role, const char *text)
{
    if (!context_mem || !cm_size) return;

    size_t used = *cm_size;
    size_t cap = AI_LOG_CAPACITY;

    char tmp[4096];
    int len = snprintf(tmp, sizeof(tmp), "%s: %s\n", role, text);
    if (len <= 0) return;

    size_t need = (size_t)len;

    while (used + need >= cap) {
        char *p = memchr(context_mem, '\n', used);
        if (!p) {
            used = 0;
            context_mem[0] = '\0';
            break;
        }

        size_t remove = (size_t)((p - context_mem) + 1);
        memmove(context_mem, context_mem + remove, used - remove);
        used -= remove;
        context_mem[used] = '\0';
    }

    memcpy(context_mem + used, tmp, need);
    used += need;
    context_mem[used] = '\0';
    *cm_size = used;
}

// 메모리에서 직접 읽어서 cJSON 배열 구성
static cJSON* build_messages_json(char *context_mem, size_t cm_size)
{
    cJSON *messages = cJSON_CreateArray();
    if (!context_mem || cm_size == 0) return messages;

    char *ptr = context_mem;
    char *end = context_mem + cm_size;

    // 메모리에서 직접 파싱 (디스크 I/O 최소화)
    while (ptr < end) {

        char *line_end = memchr(ptr, '\n', end - ptr);
        if (!line_end) line_end = end;

        size_t line_len = (size_t)(line_end - ptr);

        if (line_len > 6 && strncmp(ptr, "USER: ", 6) == 0) {
            // USER: 파싱

            char content[4096];
            size_t clen = line_len - 6;
            if (clen >= sizeof(content)) clen = sizeof(content) - 1;

            memcpy(content, ptr + 6, clen);
            content[clen] = '\0';

            cJSON *msg = cJSON_CreateObject();
            cJSON_AddStringToObject(msg, "role", "user");
            cJSON_AddStringToObject(msg, "content", content);
            cJSON_AddItemToArray(messages, msg);
        }
        else if (line_len > 11 && strncmp(ptr, "ASSISTANT: ", 11) == 0) {
            // ASSISTANT: 파싱
            char content[4096];
            size_t clen = line_len - 11;
            if (clen >= sizeof(content)) clen = sizeof(content) - 1;

            memcpy(content, ptr + 11, clen);
            content[clen] = '\0';

            cJSON *msg = cJSON_CreateObject();
            cJSON_AddStringToObject(msg, "role", "assistant");
            cJSON_AddStringToObject(msg, "content", content);
            cJSON_AddItemToArray(messages, msg);
        }

        ptr = line_end + 1;
    }

    return messages;
}

static int call_chat_api(cJSON *messages, const char *model, char *out,
                         size_t out_sz, ai_stream_cb stream_cb, void *cb_user)
{
    if (out_sz == 0) return -1;
    out[0] = '\0';

    CURL *curl = curl_easy_init();
    if (!curl) return -1;

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "model", model);
    cJSON_AddItemToObject(root, "messages", messages);
    cJSON_AddBoolToObject(root, "stream", 1);

    char *json_payload = cJSON_PrintUnformatted(root);

    ChatStreamCtx ctx = {0};
    ctx.out = out;
    ctx.out_sz = out_sz;
    ctx.stream_cb = stream_cb;
    ctx.cb_user = cb_user;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    const char *api_url = getenv("AI_HELPER_API");
    if (!api_url) api_url = CHAT_API_URL;

    curl_easy_setopt(curl, CURLOPT_URL, api_url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, stream_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ctx);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);

    CURLcode res = curl_easy_perform(curl);

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(json_payload);
    cJSON_Delete(root);

    // 마지막 JSON 처리
    if (ctx.len > 0 && !ctx.error) {
        ctx.buf = realloc(ctx.buf, ctx.len + 1);
        if (ctx.buf) {
            ctx.buf[ctx.len] = '\0';
            cJSON *parsed = cJSON_Parse(ctx.buf);
            if (parsed) {

                cJSON *msg = cJSON_GetObjectItem(parsed, "message");
                cJSON *content = msg ? cJSON_GetObjectItem(msg, "content") : NULL;

                if (content && cJSON_IsString(content)) {
                    size_t add_len = strlen(content->valuestring);

                    if (ctx.out_len + add_len + 1 < ctx.out_sz) {
                        memcpy(ctx.out + ctx.out_len,
                               content->valuestring, add_len);
                        ctx.out_len += add_len;
                        ctx.out[ctx.out_len] = '\0';
                    }
                    if (ctx.stream_cb)
                        ctx.stream_cb(content->valuestring, ctx.cb_user);
                }
                cJSON_Delete(parsed);
            }
        }
    }

    free(ctx.buf);

    if (res != CURLE_OK || ctx.error || ctx.out_len == 0)
        return -1;

    return 0;
}

int ai_chat_with_context(char *context_mem, size_t *cm_size, const char *user_input,
                         char *assistant_output, size_t out_size, const char *model)
{
    return ai_chat_with_context_stream(context_mem, cm_size, user_input,
                                       assistant_output, out_size, model,
                                       NULL, NULL);
}

int ai_chat_with_context_stream(char *context_mem, size_t *cm_size,
                                const char *user_input,
                                char *assistant_output, size_t out_size,
                                const char *model,
                                ai_stream_cb cb, void *cb_user)
{
    append_log(context_mem, cm_size, "USER", user_input);

    size_t current_sz = *cm_size;
    cJSON *messages_json = build_messages_json(context_mem, current_sz);
    if (!messages_json) return -1;

    int ret = call_chat_api(messages_json, model,
                            assistant_output, out_size,
                            cb, cb_user);

    if (ret != 0) return -1;

    append_log(context_mem, cm_size, "ASSISTANT", assistant_output);
    return 0;
}
