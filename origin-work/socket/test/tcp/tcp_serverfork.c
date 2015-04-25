/*************************************************************************
	> File Name: tcp_server.c
	> Author: 
	> Mail: 
	> Created Time: 2014年12月28日 星期日 14时02分03秒
 ************************************************************************/
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERR_EXIT(m)\
        do \
        { \
            perror(m); \
            exit(EXIT_FAILURE); \
        } while(0)

void do_service(int conn)
{
    char recvbuf[1024];
    while (1) {
        memset(recvbuf, 0, sizeof(recvbuf));
        int ret = read(conn, recvbuf, sizeof(recvbuf));
        if (ret == 0) {
            printf("client closeed\n");
            break;
        } else if (ret == -1) {
            ERR_EXIT("read");
        }
        fputs(recvbuf,stdout);
        write(conn, recvbuf, ret);
    }     
}

int main(void)
{
    int listenfd;
    pid_t pid;
    if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
   /* if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0);*/
        ERR_EXIT("socket");
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5188);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
/*    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); */
/*    inet_aton("127.0.0.1", &servaddr.sin_addr);*/

    int on = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        ERR_EXIT("setsockopt");

    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        ERR_EXIT("bind");
    }

   if(listen(listenfd, SOMAXCONN) < 0)
        ERR_EXIT("listen");

    struct sockaddr_in peeraddr;
    socklen_t peerlen = sizeof(peeraddr);
    int conn;

    while (1)
    {
        if ((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) < 0)
            ERR_EXIT("accept");

        printf("ip = %s port = %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
        
       // pid = fork();
        if ((pid = fork()) == 0) {
            close(listenfd);
            do_service(conn);
            close(conn);
            exit(EXIT_SUCCESS);
        }
        close(conn);
    }

    return 0;
}
