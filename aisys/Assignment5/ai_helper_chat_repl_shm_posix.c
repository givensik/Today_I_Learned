#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <unistd.h>
#include <fcntl.h>
#include <cjson/cJSON.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>

#define GENERATE_API_URL "http://localhost:11434/api/generate" // 단순 질의용 API URL
#define CHAT_API_URL "http://localhost:11434/api/chat" // 컨텍스트 기반 대화용 API URL
#define MODEL_NAME_QWEN3 "qwen3:0.6b" // Qwen3, r 모델이라 좀 느림
#define MODEL_NAME_GEMMA3_270M "gemma3:270m" // Gemma3, 270m 밖에 안해서 빠름
#define MODEL_NAME_GEMMA3_1B "gemma3:1b" // Gemma3 1B
#define MODEL_NAME_GPT_OSS "gpt-oss:20b" // gpu 좋은 것 있으신 분은 gpt-oss:20b 혹은 gpt-oss:120b 써보세요.

// libcurl4, cJSON 설치
// sudo apt-get install libcurl4-openssl-dev libcjson-dev
// 컴파일시 라이브러리 링크 옵션 추가 필요:
// gcc -o test_ai_helper test_ai_helper.c -lcurl -lcjson
// 혹은 ./vscode/tasks.json 에서 링크 옵션 추가(args: ["-lcurl", "-lcjson"])

// 공통으로 쓸 이름 & 구조체 (두 파일에 똑같이 넣어주세요)
#define SHM_NAME "/ai_shm"
#define SEM_TO_AI "/sem_to_ai"         // 부모 → AI
#define SEM_TO_PARENT "/sem_to_parent" // AI → 부모

typedef struct {
    char prompt[4096];   // 부모가 AI에게 보낼 질문
    char response[8192]; // AI가 부모에게 보낼 답변
} ShmBuf;



// ---------------------------------------------------------
// 함수 원형 선언
// ---------------------------------------------------------
int ai_chat_with_context(char *log_mem, size_t *log_size, const char *user_input,
                         char *assistant_output, size_t out_size, const char *model);
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);
static void append_log(char *context_mem, size_t *cm_size, const char *role, const char *text);
static cJSON* build_messages_json(char *context_mem, size_t cm_size);
static int call_chat_api(cJSON *messages, const char *model, char *out, size_t out_sz);

// libcurl 응답 저장용 구조체
typedef struct {
    char *data;
    size_t size;
} MemoryStruct;

// ---------------------------------------------------------
// main: stdin/stdout 버전 → shm + semaphore 버전으로 변경
// ---------------------------------------------------------
int main(void) {
    const char *LOG_PATH = "prompt.log";

    // prompt.log 파일 open
    int fd = open(LOG_PATH, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // mmap 초기화 (1MB)
    size_t capacity = 1024 * 1024;
    if (ftruncate(fd, capacity) != 0) {
        perror("ftruncate");
        close(fd);
        return 1;
    }

    char *context_mem =
        (char *)mmap(NULL, capacity, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (context_mem == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    size_t cm_size = 0;
    char prompt[4096];
    char response[8192];

    // ---- POSIX shm + semaphore 열기 ----
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open");
        return 1;
    }

    ShmBuf *shm = mmap(NULL, sizeof(ShmBuf),
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED) {
        perror("mmap shm");
        close(shm_fd);
        return 1;
    }

    sem_t *sem_to_ai = sem_open(SEM_TO_AI, 0);
    if (sem_to_ai == SEM_FAILED) {
        perror("sem_open to_ai");
        return 1;
    }

    sem_t *sem_to_parent = sem_open(SEM_TO_PARENT, 0);
    if (sem_to_parent == SEM_FAILED) {
        perror("sem_open to_parent");
        return 1;
    }

    // ---- 메인 루프: 부모가 세마포어로 깨우면 작업 수행 ----
    while (1) {
        // 부모가 질문을 쓸 때까지 대기
        sem_wait(sem_to_ai);

        // 공유 메모리에서 prompt 읽기
        strncpy(prompt, shm->prompt, sizeof(prompt)-1);
        prompt[sizeof(prompt)-1] = '\0';

        if (strncmp(prompt, "exit", 4) == 0) {
            break;
        }

        // 기존 컨텍스트 기반 AI 호출 그대로 사용
        int ret = ai_chat_with_context(
            context_mem,
            &cm_size,
            prompt,
            response,
            sizeof(response),
            MODEL_NAME_GEMMA3_1B
        );

        if (ret != 0) {
            snprintf(shm->response, sizeof(shm->response), "AI ERROR");
        } else {
            // 응답을 공유 메모리에 적기
            strncpy(shm->response, response, sizeof(shm->response)-1);
            shm->response[sizeof(shm->response)-1] = '\0';
        }

        // 부모에게 완료 알림
        sem_post(sem_to_parent);
    }

    // 로그 파일 정리 (원래 코드 유지)
    ftruncate(fd, cm_size);
    msync(context_mem, capacity, MS_SYNC);
    munmap(context_mem, capacity);
    close(fd);

    // shm/sem 정리
    munmap(shm, sizeof(ShmBuf));
    close(shm_fd);

    sem_close(sem_to_ai);
    sem_close(sem_to_parent);

    return 0;
}


// libcurl 콜백 함수
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;
    
    char *ptr = realloc(mem->data, mem->size + realsize + 1);
    if (!ptr) return 0;
    
    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->data[mem->size] = 0;
    
    return realsize;
}

// 메모리에 직접 로그 추가
static void append_log(char *context_mem, size_t *cm_size, const char *role, const char *text) {
    size_t max_write = 1024 * 1024 - *cm_size; // 남은 공간 계산
    if (max_write < 100) return; // 공간 부족 시 중단
    
    int len = snprintf(context_mem + *cm_size, max_write, "%s: %s\n", role, text);
    if (len > 0 && (size_t)len < max_write) {
        *cm_size += len;
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
static int call_chat_api(cJSON *messages, const char *model, char *out, size_t out_sz) {
    CURL *curl = curl_easy_init();
    if (!curl) return -1;

    // JSON payload 생성
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "model", model);
    cJSON_AddItemToObject(root, "messages", messages);
    cJSON_AddBoolToObject(root, "stream", 0);
    
    char *json_payload = cJSON_PrintUnformatted(root);
    
    // libcurl 응답 저장 구조체 초기화 (realloc이 NULL 처리하므로 malloc 불필요)
    MemoryStruct resp = {0};

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, CHAT_API_URL);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(json_payload);
    cJSON_Delete(root);

    if (res != CURLE_OK) {
        free(resp.data);
        return -1;
    }

    // 응답 데이터 유효성 체크
    if (!resp.data || resp.size == 0) {
        free(resp.data);
        return -1;
    }

    // cJSON으로 응답 파싱
    cJSON *parsed = cJSON_Parse(resp.data);
    if (!parsed) {
        free(resp.data);
        return -1;
    }

    cJSON *msg = cJSON_GetObjectItem(parsed, "message");
    cJSON *content = msg ? cJSON_GetObjectItem(msg, "content") : NULL;

    if (content && content->valuestring) {
        strncpy(out, content->valuestring, out_sz - 1);
        out[out_sz - 1] = '\0';
    } else {
        cJSON_Delete(parsed);
        free(resp.data);
        return -1;
    }

    cJSON_Delete(parsed);
    free(resp.data);
    return 0;
}

// 컨텍스트 기반 AI 대화 (메모리 주소 기반)
int ai_chat_with_context(char *context_mem, size_t *cm_size, const char *user_input,
                         char *assistant_output, size_t out_size, const char *model) {
    // 1) USER 입력 로그에 추가 (메모리 직접 쓰기)
    append_log(context_mem, cm_size, "USER", user_input);

    // 2) 메모리에서 직접 읽어서 messages JSON 생성
    cJSON *messages_json = build_messages_json(context_mem, *cm_size);
    if (!messages_json) return -1;

    // 3) /api/chat 호출 (call_chat_api가 messages_json의 소유권을 가져감)
    int ret = call_chat_api(messages_json, model, assistant_output, out_size);
    
    // ret 가 0이 아니면 에러
    if (ret != 0) return -1;

    // 4) ASSISTANT 응답 로그에 추가 (메모리 직접 쓰기)
    append_log(context_mem, cm_size, "ASSISTANT", assistant_output);

    return 0;
}

