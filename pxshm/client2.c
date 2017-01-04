#include "cliserv2.h"
#include "string.h"

int main(int argc, char **argv)
{
    int fd, i, nloop, nusec;
    pid_t pid;
    char mesg[MESGSIZE];
    long offset;
    struct shmstruct *ptr;
    
    nloop = atoi(argv[2]);
    nusec = atoi(argv[3]);

    fd = shm_open(argv[1], O_RDWR, FILE_MODE);
    ptr = mmap(NULL, sizeof(struct shmstruct), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    pid = getpid();
    for(i=0; i<nloop; i++)
    {
        sleep(nusec);
        snprintf(mesg, MESGSIZE, "pid %ld: message %d", (long)pid, i);

        if(sem_trywait(&ptr->nempty) == -1)
        {
            if(errno == EAGAIN){
                sem_wait(&ptr->noverflowmutex);
                ptr->noverflow++;
                sem_post(&ptr->noverflowmutex);
                continue;
            }
        }

        sem_wait(&ptr->mutex);
        offset = ptr->msgoff[ptr->nput];
        if(++(ptr->nput) >= NMESG)
            ptr->nput = 0;
        sem_post(&ptr->mutex);
        strcpy(&ptr->msgdata[offset], mesg);
        sem_post(&ptr->nstored);
    }

    exit(0);
}
