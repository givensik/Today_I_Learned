#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <pthread.h>

// 작업 구조체
struct job{
    struct job *next;
    struct job *prev;
    pthread_t thread_id;
};

// 큐 구조체
struct queue{
    struct job *head;
    struct job *tail;
    pthread_mutex_t lock;
};

// 큐 초기화 함수
int queue_init(struct queue *q){
    q->head = NULL;
    q->tail = NULL;
    if(pthread_mutex_init(&q->lock, NULL) != 0){
        return -1;
    }
    return 0;
}

void job_insert(struct queue *q, struct job *new_job){
    // your code here
    pthread_rwlock_wrlock(&q->lock);
    new_job->next = q->head;
    new_job->prev = NULL;
    if(q->head != NULL){ // 큐가 비어있지 않을 때 head 앞에 newjob 삽입
        q->head->prev = new_job;
    } else { // 큐가 비어있을 때 tail도 newjob으로 설정
        q->tail = new_job;
    }
    q->head = new_job;
    pthread_rwlock_unlock(&q->lock);
}

void job_append(struct queue *q, struct job *new_job){
    // your code here
    pthread_rwlock_wrlock(&q->lock);
    new_job->next = NULL;
    new_job->prev = q->tail;
    if(q->tail != NULL){ // 큐가 비어있지 않을 때 tail
        q->tail->next = new_job;
    } else { // 큐가 비어있을 때 head도 newjob으로 설정
        q->head = new_job;
    }
    q->tail = new_job;
    pthread_rwlock_unlock(&q->lock);
    
}