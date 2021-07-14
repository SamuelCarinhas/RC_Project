/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#ifndef THREAD_POOL_HEADER
#define THREAD_POOL_HEADER

#include "../data_structures/queue.h"

#include <pthread.h>

typedef struct thread_pool thread_pool_t; 
typedef struct thread_job thread_job_t;

struct thread_pool {
    size_t size, active;
    queue_t * work;
    pthread_t * pool;
    pthread_mutex_t mutex;
    pthread_cond_t signal;
};

struct thread_job {
    void * (* job)(void *);
    void * arg;
};

thread_pool_t * new_thread_pool(size_t size);
thread_job_t * new_thread_job(void * (* job)(void *), void * arg);

#endif