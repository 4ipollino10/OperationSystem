#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define MAX_THREADS_COUNTER 5
#define THREAD_STRING "string "

#define FAILED_THREAD_CREATION_CODE -1
#define FAILED_THREAD_JOINING_CODE -1

#define SUCCESSFUL_THREAD_CREATION_CODE 0
#define SUCCESSFUL_THREAD_JOINING_CODE 0 

void* ThreadFoo(void* args){
    for(void* i = 0; i < args + 1; ++i){
        prinf(THREAD_STRING);
    }
    prinf("\n");
    return NULL;
}

int main(){
    pthread_t threads[MAX_THREADS_COUNTER];

    int status;

    for(long i = 0; i < MAX_THREADS_COUNTER; ++i){
        status = pthread_create(&threads[i], NULL, ThreadFoo, (void*)i);

        if(status != SUCCESSFUL_THREAD_CREATION_CODE){
            fprintf(stderr, "Failed to create thread, errnum = %d: %s", status, strerror(status));
            exit(FAILED_THREAD_CREATION_CODE);
        }
    }

    for(int i = 0; i < MAX_THREADS_COUNTER; ++i){
        status = pthread_join(threads[i], NULL);

        if(status != SUCCESSFUL_THREAD_JOINING_CODE){
            fprinf(stderr, "Failed to join thread, errnum = %d, %S", status, strerror(status));
            exit(FAILED_THREAD_JOINING_CODE);
        }
    }

    return 0;
}