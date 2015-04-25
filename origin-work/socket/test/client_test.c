/*
 * COPYRIGHT NOTICE
 * Copyright (c) 2015
 * All rights reserved
 * 
 * @author       : colin.dj
 * @mail         : daijun163mail@163.com
 * @path         : /home/dj/my/workspace/my_prj/socket
 * @file         : client_test.c
 * @date         : 2015-04-09 12:51
 * @algorithm    : 
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h> //for gethostbyname
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_BUFF 4096
#define SERVER_PORT 12138

int 
main(int argc, char **argv)
{
    int sockfd;
    char sendBuf[MAX_BUFF], recvBUf[MAX_BUFF];
    int sendSize, recvSize;  //用于记录发送和接收数据的大小
    struct hostent *host;
    struct sockaddr_in servAddr;
    char username[32];
    char *p;

    if (argc != 3) {
        perror("use: ./client_test [hostname] username");
        exit(-1);
    }

    p = username;
    strcpy(p, argv[2]);
    printf("username:%s\n", username);
    host = gethostbyname(argv[1]);
    if (host == NULL) {
        perror("fail to get host by name.\n");
        exit(-1);
    }
    printf("success to get host by name is %s\n", inet_ntoa(*((struct in_addr *)host->h_addr)));

    //create socket 
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("failed to establish a socket\n");
        exit(1);
    }
    printf("Successful to establish a socket\n");

    /* init sockfd */
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SERVER_PORT);
    servAddr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(servAddr.sin_zero), 8);

    /* connect the socket*/
    if(connect(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr_in)) == -1) {
        perror("failed to connect the socket\n");
        exit(1);
    }
    printf("Successful to connect the socket\n");

    while(1) {
        printf("Input:");
        fgets(sendBuf, MAX_BUFF, stdin);
        send(sockfd, sendBuf, strlen(sendBuf), 0);
        memset(sendBuf, 0, sizeof(sendBuf));
        recv(sockfd, recvBUf, MAX_BUFF, 0);
        printf("Server:%s\n", recvBUf);
        memset(recvBUf, 0, sizeof(recvBUf));
    }

    return 0;
}
