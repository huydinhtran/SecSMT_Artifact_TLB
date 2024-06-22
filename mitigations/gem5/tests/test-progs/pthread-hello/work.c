#include <stdio.h>
#include <pthread.h>

void *doWork1(void *arg)
{
    for (int i=0; i<1000000000; i++)
        ;
    printf("work completed\n");
}

int main()
{
    pthread_t thread;
    int error = pthread_create(&thread, NULL, doWork1, NULL);
    if (error == 0)
    {
        printf("thread created\n");
        pthread_join(thread, NULL);
        printf("thread work complete\n");
    }
    else
    {
        printf("error in thread creation %d\n",error);
    }
    return 0;
}