#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, const char **argv)
{
    int sockfd, ret;
    struct sockaddr_in seraddr;
    char buff[32];

    bzero(&seraddr, sizeof(seraddr));

    if(inet_pton(AF_INET, argv[1], &seraddr.sin_addr) < 0)
    {
        printf("inet_pton: %s\n", strerror(errno));
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(atoi(argv[2]));

    if(connect(sockfd, (struct sockaddr*)&seraddr, sizeof(seraddr)))
    {
        printf("connect: %s\n", strerror(errno));
        return -1;
    }

    fprintf(stdout, "Input a number: ");
    if(NULL == fgets(buff, sizeof(buff), stdin))
    {
        printf("fget: %s\n", strerror(errno));
        return -1;
    }

    if(send(sockfd, buff, sizeof(buff), 0) < 0)
    {
        printf("send: %s\n", strerror(errno));
        return -1;
    }

    memset(buff, 0, sizeof(buff));
    ret = recv(sockfd, buff, sizeof(buff), 0);

    if(ret < 0)
    {
        printf("recv: %s\n", strerror(errno));
        return -1;
    }
    else if(ret > 0)
    {
        printf("Result: %s\n", buff);
    }
    else
    {
        printf("Peer shutdown!\n");
    }

    close(sockfd);
    return 0;
}

