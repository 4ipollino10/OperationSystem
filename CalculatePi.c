#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUM_OF_STEPS 200000000

#define CORRECT_AMOUNT_OF_ARGS 2
#define AMOUNT_OF_THREADS_ARG 1

#define ERROR_CODE -1
#define SUCCESS_CODE 0

#define MIN_AMOUNT_OF_THREADS 1
#define MAX_AMOUNT_OF_THREADS 30000

#define RADIX 10

#define STR_END_SYMBOL '\0'


typedef struct sumArgs
{
    int startIndex;
    int endIndex;
    double result;
} sumArgs;

void *calcPartialSum(void *arg)
{
    sumArgs *args = (sumArgs *)arg;
    double partialResult = 0;

    for (int i = args->startIndex; i < args->endIndex; ++i)
    {
        partialResult += 1.0 / (i * 4.0 + 1.0);
        partialResult -= 1.0 / (i * 4.0 + 3.0);
    }

    args->result = partialResult;
    pthread_exit(NULL);
}

int checkInput(int amountOfArgs, char* amountOfThreadsArg)
{
    if(amountOfArgs != CORRECT_AMOUNT_OF_ARGS)
    {
        printf("Expected 'one' argument\nUsing:\nEnter an integer num - amount of threads between 1 and 30000 inclusive.\n");
        return ERROR_CODE;
    }

    char* endptr;

    int amountOfThreads = (int)strtol(amountOfThreadsArg, &endptr, RADIX);
    
    if(*endptr != STR_END_SYMBOL)
    {
        printf("Expected argument type is 'integer'\nUsing:\nEnter an integer num - amount of threads between 1 and 30000 inclusive.\n");
        return ERROR_CODE;
    }

    if(amountOfThreads < MIN_AMOUNT_OF_THREADS || amountOfThreads > MAX_AMOUNT_OF_THREADS)
    {
        printf("Expected an integer number between 1 and 30000 inclusive\n");
        return ERROR_CODE;
    }
    
    if(amountOfThreads > NUM_OF_STEPS)
    {
        printf("Please enter a num less then NUM_OF_STEPS = %d\n", NUM_OF_STEPS);
        return ERROR_CODE;
    }
    
    return amountOfThreads;
}

int joinThreads(int amountOfThreads, pthread_t* threads, sumArgs* threadArgs, double* pi)
{
    int retVal = SUCCESS_CODE;

    for(int i = 0; i < amountOfThreads; ++i)
    {
        int status = pthread_join(threads[i], NULL);

        if(status != SUCCESS_CODE)
        {
            retVal = status;
        }

        *pi += threadArgs[i].result; 
    }

    return retVal;
}

int initThreads(int amountOfThreads, pthread_t* threads, sumArgs* threadsArgs)
{
    int iterationForThread = NUM_OF_STEPS / amountOfThreads;
    int residualIterations = NUM_OF_STEPS % amountOfThreads;

    for(int i = 0; i < amountOfThreads; ++i)
    {
        threadsArgs[i].startIndex = i * iterationForThread;
        threadsArgs[i].endIndex = threadsArgs[i].startIndex + iterationForThread;

        if(i < residualIterations)
        {
            ++threadsArgs[i].endIndex;
        }

        int status = pthread_create(&threads[i], NULL, calcPartialSum, (void*)&threadsArgs[i]);

        if(status != SUCCESS_CODE)
        {
            return status;
        }
    }

    return SUCCESS_CODE;
}


int calculatePi(int amountOfThreads, double* pi)
{
    if(amountOfThreads > NUM_OF_STEPS)
    {
        amountOfThreads = NUM_OF_STEPS;
    }
    
    pthread_t threads[amountOfThreads];
    sumArgs threadArgs[amountOfThreads];

    int retVal = initThreads(amountOfThreads, threads, threadArgs);

    if(retVal != SUCCESS_CODE)
    {
        fprintf(stderr, "Failed to create threads, status: %d, error: %s\n", retVal, strerror(retVal));
    }

    *pi = 0;

    retVal = joinThreads(amountOfThreads, threads, threadArgs, pi);

    if(retVal != SUCCESS_CODE)
    {
        fprintf(stderr, "Failed to join threads, status: %d, error: %s\n", retVal, strerror(retVal));
        return ERROR_CODE;
    }

    *pi *= 4.0;    

    return SUCCESS_CODE;
}


int main(int argc, char** argv)
{
    int retVal = checkInput(argc, argv[AMOUNT_OF_THREADS_ARG]);
    if(retVal == ERROR_CODE)
    {
        return EXIT_FAILURE;       
    }

    double pi = 0;

    int calcRetVal = calculatePi(retVal, &pi);

    if(calcRetVal != SUCCESS_CODE)
    {
        return EXIT_FAILURE;
    }

    printf("Your result: %.10g\n", pi);

    return EXIT_SUCCESS;
}
