#include "unistd.h"
#include "semaphore.h"
#include "sys/mman.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "fcntl.h"
#include "stdio.h"
#include "stdlib.h"

#define FILE_MODE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH

struct shmstruct {
    int count;
};
sem_t *mutex;

int main(int argc, char **argv)
{
    int fd;
    struct shmstruct *ptr;
    shm_unlink(argv[1]);

    fd = shm_open(argv[1], O_RDWR|O_CREAT|O_EXCL, FILE_MODE);
    ftruncate(fd, sizeof(struct shmstruct));
    ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    sem_unlink(argv[2]);
    mutex = sem_open(argv[2], O_CREAT|O_EXCL, FILE_MODE);
    sem_close(mutex);

    exit(0);
}
