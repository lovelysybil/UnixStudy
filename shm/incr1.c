#include "semaphore.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdio.h"
#include "stdlib.h"

#define SEM_NAME "/mysem"
#define FILE_MODE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH

int count = 0;

int main(int argc, char **argv)
{
    int i, nloop;
    sem_t *mutex;

    nloop = atoi(argv[1]);

    mutex = sem_open(SEM_NAME, O_CREAT|O_EXCL, FILE_MODE, 1);
    sem_unlink(SEM_NAME);

    setbuf(stdout, NULL);
    if(fork() == 0)
    {
        for(i=0; i<nloop; i++)
        {
            sem_wait(mutex);
            printf("child: %d\n", count++);
            sem_post(mutex);
        }

        exit(0);
    }

    for(i=0; i<nloop; i++)
    {
        for(i=0; i<nloop; i++)
        {
            sem_wait(mutex);
            printf("parent: %d\n", count++);
            sem_post(mutex);
        }
    }

    exit(0);
}
