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

void str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while(fgets(sendline, MAXLINE, fp) != NULL)
    {
        write(sockfd, sendline, strlen(sendline));

        if(readline(sockfd, recvline, MAXLINE) == 0)
            exit(0);
    
        fputs(recvline, stdout);
    }
}

int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
    str_cli(stdin, sockfd);

    exit(0);
}
