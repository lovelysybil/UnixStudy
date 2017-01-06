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
#define SERV_PORT 9877
#define SA struct sockaddr

void str_echo(int sockfd)
{
    ssize_t n;
    char buff[MAXLINE];

again:
    while((n = read(sockfd, buff, MAXLINE)) > 0)
        writen(sockfd, buff, strlen(buff));

    if(n < 0 && errno == EINTR)
        goto again;
}

int main(int argc, char **argv)
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);

    for(;;)
    {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (SA *)&cliaddr, &clilen);

        if((childpid = fork()) == 0)
        {
            char clientIp[20];
            memset(clientIp, '\0', sizeof(clientIp));
            close(listenfd);
            printf("%s:%d 已连接\n", inet_ntop(AF_INET, &cliaddr.sin_addr, clientIp, sizeof(clientIp)), ntohs(cliaddr.sin_port));
            str_echo(connfd);
            close(connfd);
            exit(0);
        }
    
        close(connfd);
    }

    exit(0);
}
