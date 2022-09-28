#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FAILED_THREAD_CREATION_CODE -1
#define FAILED_THREAD_JOINING_CODE -1
#define MAX_STR_COUNTER 10
#define SUCCESSFUL_THREAD_CREATION_CODE 0
#define SUCCESSFUL_THREAD_JOINING_CODE 0
#define CHILD_THREAD_STR "child"
#define PARENT_THREAD_STR "parent"
#define FAILED_THREAD_CREATION_ERROR_TEXT "failed to create thread\n"
#define FAILED_THREAD_JOINING_ERROR_TEXT "failed to join thread\n"

void printStrings(char* str) {
    for (int i = 0; i < MAX_STR_COUNTER; ++i) {
        printf("%s %d\n", str, i);
    }
}

void* threadFoo(void* arg){
    printStrings(CHILD_THREAD_STR);
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
	int status = pthread_create(&thread, NULL, threadFoo, NULL);

    if (status != SUCCESSFUL_THREAD_CREATION_CODE) {
        fprintf(stderr, FAILED_THREAD_CREATION_ERROR_TEXT);
        exit(FAILED_THREAD_CREATION_CODE);
    }

    void* status_addr;
    status = pthread_join(thread, &status_addr);

    if (status != SUCCESSFUL_THREAD_JOINING_CODE) {
        fprintf(stderr, FAILED_THREAD_JOINING_ERROR_TEXT);
        exit(FAILED_THREAD_JOINING_CODE);
    }

    printStrings(PARENT_THREAD_STR);

    return 0;
}
