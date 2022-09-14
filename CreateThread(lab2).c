#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FAILED_THREAD_CREATION -1
#define FAILED_THREAD_JOINING -1


void* printStrings(void *args) {
    for (int i = 10; i > 0; --i) {
        printf("child thread: %d\n", i);
    }
	
    pthread_exit((void*)1);
}

int main() {
    pthread_t thread;
    
    int status = pthread_create(&thread, NULL, printStrings, NULL);

    if (status) {
        printf(stderr, "failed to create thread\n");
        exit(FAILED_THREAD_CREATION);
    }

    void* status_addr;
    status = pthread_join(thread, &status_addr);

    if (status) {
        printf(stderr, "failed to join thread\n");
        exit(FAILED_THREAD_JOINING);
    }

    for (int i = 10; i > 0; --i) {
        printf("parent thread: %d\n", i);
    }

    return 0;
}
