#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
#include <netinet/in.h> 
#include "time.h"

#define MAXLINE 4096
#define LISTENQ 1024
#define SA struct sockaddr

int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cliaddr;
    char buff[MAXLINE];
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(13);

    bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    for(;;)
    {
        len = sizeof(cliaddr);
        connfd = accept(listenfd, (SA *)&cliaddr, &len);
        printf("connection from %s, prot %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

        write(connfd, buff, strlen(buff));

        close(connfd);
    }

    exit(0);
}
