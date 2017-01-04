#include "unistd.h"
#include "semaphore.h"
#include "sys/mman.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "fcntl.h"
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"

#define FILE_MODE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH
#define MESGSIZE 256
#define NMESG 16

struct shmstruct 
{
    sem_t mutex;
    sem_t nempty;
    sem_t nstored;

    int nput;
    long noverflow;
    sem_t noverflowmutex;
    long msgoff[NMESG];
    char msgdata[MESGSIZE*NMESG];
};
