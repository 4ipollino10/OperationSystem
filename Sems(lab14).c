#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>

#define AMOUNT_OF_SEMAPHORES 2
#define SUCCES_CODE 0
#define ERROR_CODE -1
#define AMOUNT_OF_STRS 10
#define MAIN_START_SEMAPHORE_INDEX 1
#define CHILD_START_SEMAPHORE_INDEX 0
#define SEMAPHORE_PSHARED_MODE 0


sem_t semaphores[AMOUNT_OF_SEMAPHORES];

typedef struct threadArgs 
{
    const char* text;
    int startSemaphoreIndex;
}threadArgs;


int initSems()
{
    for(int i = 0; i < AMOUNT_OF_SEMAPHORES; ++i)
    {
        int init_code = sem_init(&semaphores[i], SEMAPHORE_PSHARED_MODE, i);
        if(init_code != SUCCES_CODE)
        {
            perror("Semaphore initialization error");
            return ERROR_CODE;
        }
    }

    return SUCCES_CODE;
}

int destroySems()
{
    for(int i = 0; i < AMOUNT_OF_SEMAPHORES; ++i)
    {
        int postCode = sem_post(&semaphores[i]);
        if(postCode != SUCCES_CODE)
        {
            perror("Semaphore post error");
            return ERROR_CODE;
        }

        int destroyCode = sem_destroy(&semaphores[i]);
        if(destroyCode != SUCCES_CODE){
            perror("Semaphore destruction error");
            return ERROR_CODE;
        }
    }
    
    return SUCCES_CODE;
}

int semaphoreWait(int sem)
{
    int semWaitCode = sem_wait(&semaphores[sem]);
    if(semWaitCode != SUCCES_CODE)
    {
        fprintf(stderr, "Failed to sem_wait(), status: %d, error: %s\n", semWaitCode, strerror(semWaitCode));
        return ERROR_CODE;
    }

    return SUCCES_CODE;
}

int semaphorePost(int sem)
{
   int semWaitCode = sem_post(&semaphores[sem]);
    if(semWaitCode != SUCCES_CODE)
    {
        fprintf(stderr, "Failed to sem_post(), status: %d, error: %s\n", semWaitCode, strerror(semWaitCode));
        return ERROR_CODE;
    }

    return SUCCES_CODE; 
}

void* printText(void* args)
{
    threadArgs* arg = (threadArgs*)args;
    
    int curSem = 0;
    int nextSem = 0;

    for(int i = 0; i < AMOUNT_OF_STRS; ++i)
    {
        curSem = (arg->startSemaphoreIndex + 1) % AMOUNT_OF_SEMAPHORES;
        nextSem = (curSem + 1) % AMOUNT_OF_SEMAPHORES;

        int waitSemCode = semaphoreWait(curSem);
        if(waitSemCode != SUCCES_CODE)
        {
            perror("Semaphore wait error");
            return NULL;
        }

        printf("%s %d\n", arg->text, i);

        int postSemCode = semaphorePost(nextSem);
        if(waitSemCode != SUCCES_CODE)
        {
            perror("Semaphore post error");
            return NULL;
        }

    }

    return NULL;
}

int main() 
{
    pthread_t thread;
    
    threadArgs mainThreadArgs = {"Main thread", MAIN_START_SEMAPHORE_INDEX};
    threadArgs childThreadArgs = {"Child thread", CHILD_START_SEMAPHORE_INDEX};

    int initCode = initSems();
    if(initCode != SUCCES_CODE)
    {
        destroySems(AMOUNT_OF_SEMAPHORES);
        exit(initCode);
    }

    int status = pthread_create(&thread, NULL, printText, &childThreadArgs);
    if(status != SUCCES_CODE)
    {
        fprintf(stderr, "Failed to create thread, status: %d, error: %s\n", status, strerror(status));
        destroySems();
        exit(status);
    }

    printText(&mainThreadArgs);

    status = pthread_join(thread, NULL);
    if(status != SUCCES_CODE) 
    {
        fprintf(stderr, "Failed to join thread, status: %d, error: %s\n", status, strerror(status));
        destroySems();
        exit(status);
    }

    destroySems();
    
    return SUCCES_CODE;
}