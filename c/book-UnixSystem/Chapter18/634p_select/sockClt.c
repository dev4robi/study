#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MSGLEN 32

int main(int argc, char **argv)
{
    int sockFd, addrLen, result;
    struct sockaddr_in sockAddr;
    char readbuf[MSGLEN], sendbuf[MSGLEN];

    int retSelect;
    struct timeval tv;
    fd_set rset;

    tv.tv_sec = 10;
    tv.tv_usec = 0;

    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockFd < 0)
    {
        fprintf(stderr, "Socket open error.\n");
        exit(1);
    }

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = 9999;
    addrLen = sizeof(sockAddr);

    if (connect(sockFd, (struct sockaddr *)&sockAddr, addrLen) == -1)
    {
        fprintf(stderr, "Fail to connect to server.\n");
        exit(1);
    }

    while (1)
    {
        fprintf(stdout, "SendMsg: ");
        fgets(sendbuf, MSGLEN, stdin);
        write(sockFd, sendbuf, MSGLEN);

        if (!strncmp(sendbuf, "quit", 4)) break;

        FD_ZERO(&rset);
        FD_SET(sockFd, &rset);
        retSelect = select(sockFd + 1, &rset, NULL, NULL, &tv);

        if (retSelect == 0)
        {
            fprintf(stderr, "... TIME OUT ...\n");
            break;
        }

        read(sockFd, readbuf, MSGLEN);
        fprintf(stdout, "RecvMsg: %s", readbuf);
    }

    close(sockFd);
    exit(0);
}