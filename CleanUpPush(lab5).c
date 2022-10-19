#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define SUCCSESSFULL_THREAD_CREATION_CODE 0
#define SUCCSESSFULL_THREAD_CANCELATION_CODE 0
#define SUCCSESSFULL_PROGRAM_COMPLETION_CODE 0
#define SUCCESSFUL_THREAD_JOINING_CODE 0

#define FAILED_THREAD_CREATION_EXIT_CODE -1
#define FAILED_THREAD_CANCELATION_EXIT_CODE -1
#define FAILED_THREAD_JOINING_EXIT_CODE -1

#define TRUE_STATMENT 1
#define THREAD_STR "Child thread\n"
#define THREAD_TERIMATING_STR "Thread %lu is terminated\n"
#define SLEEP_TIME 2

#define PARENT_THREAD_STR1 "Starting cancellation\n"
#define PARENT_THREAD_STR2 "Child thread has been cancelled\n"

void onExit(void* args){
    printf(THREAD_TERIMATING_STR, pthread_self());
}

void* threadFoo(void* args){
    pthread_cleanup_push(onExit, NULL);
    while(TRUE_STATMENT){
        printf(THREAD_STR);
    }
    pthread_cleanup_pop();
    pthread_exit(0);
}

int main(){
    pthread_t thread;

    int status = pthread_create(&thread, NULL, threadFoo, NULL);

    if(status != SUCCSESSFULL_THREAD_CREATION_CODE){
        fprintf(stderr, "Failed to create thread, status: %d, error: %s\n", status, strerror(status));
        exit(FAILED_THREAD_CREATION_EXIT_CODE);
    }

    sleep(SLEEP_TIME);

    printf(PARENT_THREAD_STR1);

    status = pthread_cancel(thread);

    if(status != SUCCSESSFULL_THREAD_CANCELATION_CODE){
        fprintf(stderr, "Failed to cancel thread, status: %d, error: %s\n", status, strerror(status));
        exit(FAILED_THREAD_CANCELATION_EXIT_CODE);
    }

    status = pthread_join(thread, NULL);

    if(status != SUCCESSFUL_THREAD_JOINING_CODE){
        fprintf(stderr, "Failed to join thread, status: %d, error: %s\n", status, strerror(status));
        exit(FAILED_THREAD_JOINING_EXIT_CODE);
    }

    printf(PARENT_THREAD_STR2);
    
    exit(SUCCSESSFULL_PROGRAM_COMPLETION_CODE);
}