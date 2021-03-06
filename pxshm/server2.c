#include "cliserv2.h"

int main(int argc, char **argv)
{
    int fd, index, lastnoverflow, temp;
    long offset;
    struct shmstruct *ptr;

    shm_unlink(argv[1]);
    fd = shm_open(argv[1], O_RDWR|O_CREAT|O_EXCL, FILE_MODE);
    ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    ftruncate(fd, sizeof(struct shmstruct));
    close(fd);

    for(index=0; index<NMESG; index++)
        ptr->msgoff[index] = index*MESGSIZE;

    sem_init(&ptr->mutex, 1, 1);
    sem_init(&ptr->nempty, 1, NMESG);
    sem_init(&ptr->nstored, 1, 0);
    sem_init(&ptr->noverflowmutex, 1, 1);

    index = 0;
    lastnoverflow = 0;
    for(;;)
    {
        sem_wait(&ptr->nstored);
        sem_wait(&ptr->mutex);
        offset = ptr->msgoff[index];
        printf("index = %d: %s\n", index, &ptr->msgdata[offset]);
        if(++index >= NMESG)
            index = 0;

        sem_post(&ptr->mutex);
        sem_post(&ptr->nempty);

        sem_wait(&ptr->noverflowmutex);
        temp = ptr->noverflow;
        sem_post(&ptr->noverflowmutex);
        if(temp != lastnoverflow)
        {
            printf("noverflow = %d\n", temp);
            lastnoverflow = temp;
        }
    }

    exit(0);
}
