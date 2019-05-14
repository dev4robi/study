#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MSGLEN 32

int main()
{
    int sockFd, addrLen, result;
    struct sockaddr_in sockAddr;
    char msgbuf[MSGLEN];

    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = 9999;

    addrLen = sizeof(sockAddr);
    result = connect(sockFd, (struct sockaddr *)&sockAddr, addrLen);

    if (result == -1)
    {
        fprintf(stderr, "Fail to connect server!\n");
        exit(1);
    }

    printf("MSG: ");
    fgets(msgbuf, MSGLEN, stdin);

    write(sockFd, msgbuf, MSGLEN);
    read(sockFd, msgbuf, MSGLEN);

    printf("MSG ECHO: %s\n", msgbuf);
    close(sockFd);
    exit(0);
}