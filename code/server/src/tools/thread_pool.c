/**
 * Project: RC 2021
 * 
 * Authors:
 *  Samuel dos Santos Carinhas - 2019217199
 *  Carlos Eduardo da Costa Jordão - 2019221373
 * 
 */

#include "thread_pool.h"

void * thread_job(void * arg);

thread_pool_t * new_thread_pool(size_t size) {
    thread_pool_t * thread_pool = malloc(sizeof(thread_pool_t));

    thread_pool->size = size;
    thread_pool->active = 1;
    thread_pool->pool = (pthread_t *) malloc(sizeof(pthread_t) * size);

    for(size_t i = 0; i < size; i++) {
        pthread_create(&thread_pool->pool[i], NULL, thread_job, NULL);
    }

    thread_pool->work = new_queue();
    thread_pool->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    thread_pool->signal = (pthread_cond_t) PTHREAD_COND_INITIALIZER;

    return thread_pool;
}

void remove_thread_pool(thread_pool_t * thread_pool) {
    thread_pool->active = 0;
    
    for(size_t i = 0; i < thread_pool->size; i++) {
        pthread_cond_signal(&thread_pool->signal);
    }

    for(size_t i = 0; i < thread_pool->size; i++) {
        pthread_join(thread_pool->pool[i], NULL);
    }

    remove_queue(thread_pool->work);
    free(thread_pool->pool);
    free(thread_pool);
}

thread_job_t * new_thread_job(void * (* job)(void * arg), void * arg) {
    thread_job_t * thread_job = malloc(sizeof(thread_job_t));
    
    thread_job->job = job;
    thread_job->arg = arg;
    
    return thread_job;
}

void * thread_job(void * arg) {
    thread_pool_t * thread_pool = (thread_pool_t *) arg;

    while(thread_pool->active == 1) {
        pthread_mutex_lock(&thread_pool->mutex);

        pthread_cond_wait(&thread_pool->signal, &thread_pool->mutex);

        thread_job_t * thread_job = (thread_job_t *) queue_peek(thread_pool->work);
        queue_pop(thread_pool->work);

        pthread_mutex_unlock(&thread_pool->mutex);
    
        if(thread_job->job) {
            thread_job->job(thread_job->arg);
        }
    }

    return NULL;
}

void add_thread_job(thread_pool_t * thread_pool, thread_job_t * thread_job) {
    queue_node_t * node = new_queue_node(thread_job, sizeof(thread_job_t));
    queue_push(thread_pool->work, node);
}
