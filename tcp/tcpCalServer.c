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

void accept_handle(int connfd, struct sockaddr_in peeraddr)
{
    char buf[32];
    int val;

    printf("Receive request from %s, port %d\n", inet_ntop(AF_INET, &peeraddr.sin_addr, buf, \
                                                           sizeof(buf)), ntohs(peeraddr.sin_port));

    memset(buf, 0, sizeof(buf));
    if(recv(connfd, buf, sizeof(buf), 0) < 0)
    {
        printf("recv: %s\n", strerror(errno));
        return;
    }

    val = atoi(buf);
    val *= val;
    snprintf(buf, sizeof(buf), "%d", val);
    if(send(connfd, buf, sizeof(buf), 0) < 0)
    {
        printf("send: %s\n", strerror(errno));
        return;
    }
}

void sig_chld()
{
    pid_t pid;

    while((pid = waitpid(-1, NULL, WNOHANG)) > 0)
    {
        printf("Child %d terminated\n", pid);
    }
}

int main(int argc, char **argv)
{
    int sockfd, connfd;
    struct sockaddr_in seraddr, peeraddr;
    socklen_t addrlen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&seraddr, sizeof(seraddr));

    seraddr.sin_family = AF_INET;
    seraddr.sin_port = htons(atoi(argv[1]));
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)))
    {
        printf("bind: %s\n", strerror(errno));
        return -1;
    }

    if(listen(sockfd, 100) < 0)
    {
        printf("listen: %s\n", strerror(errno));
        return -1;
    }

    signal(SIGCHLD, sig_chld);

    addrlen = sizeof(struct sockaddr_in);
    for(;;)
    {
        connfd = accept(sockfd, (struct sockaddr*)&peeraddr, &addrlen);
        if(connfd < 0)
        {
            printf("accept: %s\n", strerror(errno));
            continue;
        }

        if(0 == fork())
        {
            close(sockfd);
            accept_handle(connfd, peeraddr);
            close(connfd);
            exit(0);
        }

        close(connfd);
    }

    return 0;
}
