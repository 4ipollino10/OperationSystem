#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FAILED_THREAD_CREATION -1
#define MAX_STR_COUNTER 10
#define SUCCESSFUL_THREAD_CREATION_CODE 0
#define CHILD_THREAD_STR "child"
#define PARENT_THREAD_STR "parent"
#define FAILED_THREAD_CREATION_ERROR_TEXT "failed to create thread\n"

void printStrings(char* str) {
    for (int i = 0; i < MAX_STR_COUNTER; ++i) {
        printf("%s %d\n", str, i);
    }
}

void* threadFoo(void* arg){
    printStrings(CHILD_THREAD_STR);
    return NULL;
}

int main() {
    pthread_t thread;
    
    int status = pthread_create(&thread, NULL, threadFoo, NULL);

    if (status != SUCCESSFUL_THREAD_CREATION_CODE) {
        printf(stderr, FAILED_THREAD_CREATION_ERROR_TEXT);
        exit(FAILED_THREAD_CREATION);
    }
    
    printStrings(PARENT_THREAD_STR);

    void* retVal;
    pthread_exit(retVal);
	
    return 0;
}