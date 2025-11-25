#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    const char *name;
    const char *owner;
    int owner_idx;
    pthread_mutex_t mutex;
} Mutex;

typedef struct {
    int edge[2][2];
    pthread_mutex_t lock;
} wait_for_graph_t;

wait_for_graph_t wait_for_graph = {
    .edge = {{0}},
    .lock = PTHREAD_MUTEX_INITIALIZER,
};

Mutex mutex_a = {.name = "A", .owner = NULL, .owner_idx = -1, .mutex = PTHREAD_MUTEX_INITIALIZER};
Mutex mutex_b = {.name = "B", .owner = NULL, .owner_idx = -1, .mutex = PTHREAD_MUTEX_INITIALIZER};
time_t deadlock_started = 0;

const int THREAD_COUNT = 2;
const char *THREAD_LABELS[] = {"T1(A->B)", "T2(B->A)"};

void clear_edges_from(int from) {
    for (int i = 0; i < THREAD_COUNT; i++) {
        wait_for_graph.edge[from][i] = 0;
    }
}

void add_edge(int from, int to, const char *waiter, const char *holder, const char *resource) {
    pthread_mutex_lock(&wait_for_graph.lock);
    if (wait_for_graph.edge[from][to] == 0) {
        wait_for_graph.edge[from][to] = 1;
        printf("[감시기] 스레드 %s -> 스레드 %s를 기다림 (자원 %s)\n", waiter, holder, resource);
    }
    pthread_mutex_unlock(&wait_for_graph.lock);
}

bool dfs_cycle(int node, bool *visited, bool *stack) {
    visited[node] = true; // 방문 표시
    stack[node] = true; // 현재 경로에 포함 표시
    for (int next = 0; next < THREAD_COUNT; next++) {
        if (!wait_for_graph.edge[node][next]) {
            continue;
        }
        if (!visited[next] && dfs_cycle(next, visited, stack)) {
            return true;
        }
        if (stack[next]) {
            return true;
        }
    }
    stack[node] = false;
    return false;
}

bool detect_cycle(void) {
    bool visited[2] = {false, false}; // 방문 여부 배열
    bool stack[2] = {false, false}; // 현재 경로 포함 여부 배열
    for (int i = 0; i < THREAD_COUNT; i++) {
        if (!visited[i] && dfs_cycle(i, visited, stack)) {
            return true;
        }
    }
    return false;
}

void *monitor_cycle(void *arg) {
    while (1) {
        
        pthread_mutex_lock(&wait_for_graph.lock); // wait-for 그래프 접근을 위한 락
        bool has_cycle = detect_cycle(); // 데드락(사이클) 감지
        
        // 현재 시간과 데드락 시작 시간 계산
        time_t now = time(NULL); // 현재 시간 얻기
        // 데드락이 존재하면 deadlock_started 타임스탬프 설정
        if (has_cycle) {
            if (deadlock_started == 0) {
                deadlock_started = now;
            }
        } else {
            deadlock_started = 0;
        }
        time_t started = deadlock_started;

        pthread_mutex_unlock(&wait_for_graph.lock); // wait-for 그래프 접근 완료 후 락 해제

        // 데드락이 5초 이상 지속된 경우
        if (started != 0 && now - started >= 5) {
            printf("\n=== 데드락 감지됨! (5초 경과) ===\n");
            printf("  감지된 스레드들 :\n");
            // 데드락에 관련된 스레드 출력
            for (int i = 0; i < THREAD_COUNT; i++) {
                for (int j = 0; j < THREAD_COUNT; j++) {
                    if (wait_for_graph.edge[i][j]) {
                        printf("      - 스레드 %s\n", THREAD_LABELS[i]);
                        break;
                    }
                }
            }
            printf("\n프로그램을 종료합니다.\n");
            exit(0);
        }

        usleep(200000);
    }
    return NULL;
}

void lock_mutex(const char *label, int idx, Mutex *res) {

    printf("[스레드 %s] 자원 %s 잠금 시도\n", label, res->name);
    
    while (1) {
        int ret = pthread_mutex_trylock(&res->mutex); // 논블로킹 잠금 시도
        if (ret == 0) { // 잠금 성공했을 때
            res->owner = label; // 소유자 기록
            res->owner_idx = idx; // 소유자 인덱스 기록
            pthread_mutex_lock(&wait_for_graph.lock); // wait-for 그래프 수정을 위한 락
            clear_edges_from(idx);
            pthread_mutex_unlock(&wait_for_graph.lock); // wait-for 그래프 수정 완료 후 락 해제
            printf("[스레드 %s] 자원 %s 잠금 성공\n", label, res->name);
            return;
        }
        // 잠금 실패했을 때 (EBUSY일 때)
        if (ret == EBUSY && res->owner != NULL && res->owner_idx >= 0) {
            add_edge(idx, res->owner_idx, label, res->owner, res->name);
            // printf("[스레드 %s] 자원 %s 잠금 실패, 스레드 %s가 소유 중\n", label, res->name, res->owner);
        }
        usleep(200000);
    }
}

// 스레드 1 : A -> B
void *thread1_work(void *arg) {
    const char *label = THREAD_LABELS[0];
    int idx = 0;
    lock_mutex(label, idx, &mutex_a); // A 잠금
    sleep(1);
    lock_mutex(label, idx, &mutex_b); // B 잠금
    return NULL;
}

// 스레드 2 : B -> A
void *thread2_work(void *arg) {
    const char *label = THREAD_LABELS[1];
    int idx = 1;
    lock_mutex(label, idx, &mutex_b); // B 잠금
    sleep(1);
    lock_mutex(label, idx, &mutex_a); // A 잠금
    return NULL;
}

int main(void) {
    pthread_t threads[2];
    pthread_t monitor;

    pthread_create(&monitor, NULL, monitor_cycle, NULL);
    pthread_create(&threads[0], NULL, thread1_work, NULL);
    pthread_create(&threads[1], NULL, thread2_work, NULL);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_cancel(monitor);
    pthread_join(monitor, NULL);

    return 0;
}
