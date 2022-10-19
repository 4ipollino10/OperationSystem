#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define TRUE_STATMENT 1
#define THERAD_STR "Child thread\n"
#define SLEEP_TIME 2

#define PARENT_THREAD_STR1 "Starting cancellation\n"
#define PARENT_THREAD_STR2 "Child thread has been cancelled\n"

#define SUCCSESSFULL_THREAD_CREATION_CODE 0
#define SUCCSESSFULL_THREAD_JOINING_CODE 0
#define SUCCSESSFULL_THREAD_CANCELATION_CODE 0
#define SUCCSESSFULL_PROGRAM_COMPLETION_CODE 0

#define FAILED_THREAD_CREATION_EXIT_CODE -1
#define FAILED_THREAD_CANCELATION_EXIT_CODE -1
#define FAILED_THREAD_JOINING_EXIT_CODE -1



void* ThreadFoo(void* args){
    while(1){
        printf(THREAD_STR);
    }
}

int main(){
    pthread_t thread;

    int status = pthread_create(&thread, NULL, ThreadFoo, NULL);

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
	
    void* retVal;
	
    status = pthread_join(thread, retVal);
	
    if(status != SUCCSESSFULL_THREAD_JOINING_CODE){
	fprintf(stderr, "Failed to join thread, status: %d, error: %s\n", status, strerror(status));
	exit(FAILED_THREAD_JOINING_EXIT_CODE);
    }
	
    if(retVal != PTHREAD_CANCELLED){
	fprintf(stderr, "Thread wasn't cancelled by pthread_cancel()");
	exit(FAILED_THREAD_CANCELATION_EXIT_CODE);
    }
	
    printf(PARENT_THREAD_STR2);
    
    exit(SUCCSESSFULL_PROGRAM_COMPLETION_CODE);
}
