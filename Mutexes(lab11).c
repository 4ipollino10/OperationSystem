#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#define ERROR_CODE -1
#define SUCCESS_CODE 0

#define AMOUNT_OF_MUTEXES 3
#define START_MUTEX 0

#define STR_COUNT 10

typedef struct threadAargs
{
    const char* str;
}threadArgs;

pthread_mutex_t mutexArr[AMOUNT_OF_MUTEXES];

bool preaparationFlag = false;

int lockMutex(int mutex)
{
    int status = pthread_mutex_lock(&mutexArr[mutex]);
    if(status != SUCCESS_CODE)
    {
        fprintf(stderr, "Failed to lock mutex, status: %d, error: %s\n", status, strerror(status));
        return ERROR_CODE;
    }

    return SUCCESS_CODE;
}

int unlockMutex(int mutex)
{
    int status = pthread_mutex_unlock(&mutexArr[mutex]);
    if(status != SUCCESS_CODE)
    {
        fprintf(stderr, "Failed to unlock mutex, status: %d, error: %s\n", status, strerror(status));
        return ERROR_CODE;
    }

    return SUCCESS_CODE;
}


int initMutexes() 
{
    pthread_mutexattr_t mutexAttr;

    int status = pthread_mutexattr_init(&mutexAttr);
    if(status != SUCCESS_CODE)
    {
        fprintf(stderr, "Failed to init mutex attributes, status: %d, error: %s\n", status, strerror(status));
        return ERROR_CODE;
    }

    status = pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_ERRORCHECK);
    if(status != SUCCESS_CODE)
    {
        fprintf(stderr, "Failed to set mutex attributes, status: %d, error: %s\n", status, strerror(status));
        return ERROR_CODE;
    }

    for(int i = 0; i < AMOUNT_OF_MUTEXES; ++i)
    {
        status = ptread_mutex_init(&mutexArr[i], &mutexAttr);
        if(status != SUCCESS_CODE)
        {
            fprintf(stderr, "Failed to initialize mutex, status: %d, error: %s\n", status, strerror(status));
            return ERROR_CODE;
        }
    }

    return SUCCESS_CODE;
}

void destroyMutexes()
{
    for(int i = 0; i < AMOUNT_OF_MUTEXES; ++i){
        int status = pthread_mutex_destroy(&mutexArr[i]);
        if(status != SUCCESS_CODE)
        {
            fprintf(stderr, "Failed to destroy mutex, status: %s, error: %s\n", status, strerror(status));
        }
    }
}

void* printText(void* args)
{
    threadAargs* arg = (threadAargs*)args;

    int lockStatus, unlockStatus;
    int curMutex = START_MUTEX;
    int nextMutex = START_MUTEX;

    if(!preaparationFlag)
    {
        curMutex = 2;
        lockStatus = lockMutex(curMutex);
        if(lockStatus != SUCCESS_CODE)
        {
            return NULL;
        }
        
        preaparationFlag = true;
    }

    for(int i = 0; i < STR_COUNT; ++i)
    {
        nextMutex = (curMutex + 1) % AMOUNT_OF_MUTEXES;

        lockStatus = lockMutex(nextMutex);
        if(lockStatus != SUCCESS_CODE)
        {
            return NULL;
        }

        printf("%s%d", arg->str, i);

        unlockStatus = unlockMutex(curMutex);
        if(unlockStatus != SUCCESS_CODE)
        {
            return NULL;
        }

        curMutex = nextMutex;
    }

    unlockMutex(curMutex);
        
    return NULL;
}

int main() 
{
    pthread_t thread;
    
    threadArgs mainThreadArg = {"main thread "};
    threadArgs childThreadArg = {"child thread "};

    int initStatus = initMutexes();
    if(initStatus != SUCCESS_CODE)
    {
        destroyMutexes();
        return ERROR_CODE;
    }

    int lockStatus = lockMutex(START_MUTEX);
    if(lockStatus != SUCCESS_CODE)
    {
        destroyMutexes();
        return ERROR_CODE;
    }

    int createStatus = pthread_create(&thread, NULL, printText, (void*)&childThreadArg);
    if(createStatus != SUCCESS_CODE)
    {
        fprintf(stderr, "Failed to create thread, status: %d, error: %s\n", createStatus, strerror(createStatus));
    }

    while(preaparationFlag != true) {}

    printText((void*)&mainThreadArg);

    int joinStatus = pthread_join(thread, NULL);
    if(joinStatus != SUCCESS_CODE)
    {
        fprintf(stderr, "Failed to join thread, status: %d, error: %s\n", joinStatus, strerror(joinStatus));
    }

    destroyMutexes();

    return SUCCESS_CODE;
}




