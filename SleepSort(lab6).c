#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAX_STR_AMOUNT 100
#define COFFICIENT = 100000

#define SUCCESSFULL_PROGRAMM_EXIT_CODE 0
#define SUCCESSFULL_THREAD_CREATION_CODE 0
#define SUCCESSFULL_THREAD_JOINING_CODE 0
#define SUCCESSFULL_SLEEP_CODE 0

#define ERROR_PROGRAMM_EXIT_CODE -1
#define ERROR_GETLINE_CODE -1
#define ERROR_EXIT_READ_CODE -1

void* printString(void* arg){
    int status = usleep(COFFICIENT * strlen((char *)arg));
    
    if(status != SUCCESSFULL_SLEEP_CODE){
        fprintf(stderr, "usleep() error, status: %d, error: %s\n", strerror(status));
    }

    printf("%s",(char *)arg);
    
    return NULL;
}

void freeStrings(char** strings, int strCounter){
    for(int i = 0; i < strCounter; ++i){
        free(strings[i]);
    }
}

int readStrings(char** strings){
    int readSymbols = 2;
    int strIndex = 0;
    
    size_t strLength = 0;

    while(readSymbols > 1 && strIndex < 100){
        int readSymbols = getline(strings[strIndex], &strLength, stdin);
       
        if(readSymbols == ERROR_GETLINE_CODE){
            fprintf(stderr, "getline() error: %s\n", strerror(errno));
            
            return ERROR_EXIT_READ_CODE;
        }

        strIndex++;
    }

    return  strIndex--;
}

int main(){
    char* strings[MAX_STR_AMOUNT];

    int strCounter = readStrings(strings);

    if(strCounter == ERROR_EXIT_READ_CODE){
        freeStrings(strings, strCounter);
        
        return ERROR_PROGRAMM_EXIT_CODE;
    }

    pthread_t *threads = malloc(sizeof(pthread_t) * strCounter);

    if(threads == NULL){
        fprintf(stderr, "malloc() error: %s\n", strerror(errno));
        
        freeStrings(strings, strCounter);
        
        return ERROR_PROGRAMM_EXIT_CODE;
    }

    for(int i = 0; i < strCounter; ++i){
        int status = phread_create(&threads[i], NULL, printString, strings[i]);
        
        if(status != SUCCESSFULL_THREAD_CREATION_CODE){
            fprintf(stderr, "Failed to create thread, status: %d, error: %s\n", status, strerror(status));
            
            break;
        }
    }

    for(int i = 0; i < strCounter; ++i){
        int status = pthread_join(threads[i], NULL);

        if(status != SUCCESSFULL_THREAD_JOINING_CODE){
            fprintf(stderr, "Failed to join thread, status: %d, error: %s\n", status, strerror(status));
        }
    }

    freeStrings(strings, strCounter);
    free(threads);

    return SUCCESSFULL_PROGRAMM_EXIT_CODE;
}