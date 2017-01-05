#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
#include <netinet/in.h> 

#define MAXLINE 4096
#define LISTENQ 1024
#define SA struct sockaddr

int main(int argc, char **argv)
{
    int sockfd, n;
    char recvline[MAXLINE+1];
    struct sockaddr_in servaddr;
    
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        printf("socket error\n");

    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        printf("inet_pton error for %s\n", argv[1]);

    if(connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) < 0)
        printf("connect error\n");

    while((n = read(sockfd, recvline, MAXLINE)) > 0)
    {
        recvline[n] = '\0';
        if(fputs(recvline, stdout) == EOF)
            printf("fputs error\n");
    
    }

    exit(0);
}
