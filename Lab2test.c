#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void* threadFoo1(void* arg){
    sleep(3);
    
    printf("child thread woke up\n");
    
    return NULL;
}


void* threadFoo2(void* arg){
    pthread_t* thread = (pthread_t*)arg;

    sleep(4);
    
    int status = pthread_join(*thread, NULL);
    
    printf("child thread join status = %d", status);
    
    if(status != 0){
        printf("error: %s\n", strerror(status));
    }
}

int main(){
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, threadFoo1, NULL);
    pthread_create(&thread2, NULL, threadFoo2, &thread1);

    sleep(4);

    int status = pthread_join(thread1, NULL);
    
    printf("main thread join status = %d", status);
    
    if(status != 0){
        printf("error: %s\n", strerror(status));
    }
    
    pthread_exit((void *)0);
}
