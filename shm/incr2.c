#include "semaphore.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "stdio.h"
#include "stdlib.h"

#define SEM_NAME "/mysem"
#define FILE_MODE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH

int main(int argc, char **argv)
{
    int fd, i, nloop, zero=0;
    int *ptr;
    sem_t *mutex;

    nloop = atoi(argv[2]);
    fd = open(argv[1], O_RDWR|O_CREAT, FILE_MODE);
    write(fd, &zero, sizeof(int));
    ptr = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    mutex = sem_open(SEM_NAME, O_CREAT|O_EXCL, FILE_MODE, 1);
    sem_unlink(SEM_NAME);

    setbuf(stdout, NULL);
    if(fork() == 0)
    {
        for(i=0; i<nloop; i++)
        {
            sem_wait(mutex);
            printf("child: %d\n", (*ptr)++);
            sem_post(mutex);
        }

        exit(0);
    }

    for(i=0; i<nloop; i++)
    {
        for(i=0; i<nloop; i++)
        {
            sem_wait(mutex);
            printf("parent: %d\n", (*ptr)++);
            sem_post(mutex);
        }
    }

    exit(0);
}
