#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void* threadFoo1(void* arg){
    sleep(3);
    return NULL;
}


void* threadFoo2(void* arg){
    pthread_t* thread = (pthread_t*)arg;
    printf("thread: %d\n", *thread);

    sleep(1);
    
    int status = pthread_join(*thread, NULL);

    if(status != 0){
        printf("error: %s\n", strerror(status));
    }
}

int main(){
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, threadFoo1, NULL);

    pthread_create(&thread2, NULL, threadFoo2, &thread1);

    sleep(1);

    int status = pthread_join(thread1, NULL);
    
    if(status != 0){
        printf("error: %s\n", strerror(status));
    }
    
    pthread_exit(0);
}