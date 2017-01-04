#include "unistd.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>

struct mq_attr attr;

int main(int argc, char **argv)
{
    int c, flags;
    mqd_t mqd;
    flags = O_RDWR | O_CREAT;
    while((c = getopt(argc, argv, "em:z:")) != -1)
    {
        switch(c)
        {
        case 'e':
            flags |= O_EXCL;
            break;

        case 'm':
            attr.mq_maxmsg = atol(optarg);
            break;

        case 'z':
            attr.mq_msgsize = atol(optarg);
            break;
        }
    }

    mqd = mq_open(argv[optind], flags, FILE_MODE, (attr.mq_maxmsg != 0) ? &attr:NULL);
    mq_close(mqd);

    exit(0);
}
