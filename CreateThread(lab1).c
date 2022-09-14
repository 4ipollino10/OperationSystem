#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FAILED_THREAD_CREATION -1

void* printStrings(void *args) {
    for (int i = 10; i > 0; --i) {
        printf("child thread gonna call mama in %d\n", i);
    }
}

int main() {
    pthread_t thread;
    
    int status = pthread_create(&thread, NULL, printStrings, NULL);

    if (status != 0) {
        printf(stderr,"failed to create thread");
        exit(FAILED_THREAD_CREATION);
    }
	
    for (int i = 10; i > 0; --i) {
        printf("parent thread gonna call mama in %d\n", i);
    }
    
    void* retVal;
    pthread_exit(retVal);
	
    return 0;
}