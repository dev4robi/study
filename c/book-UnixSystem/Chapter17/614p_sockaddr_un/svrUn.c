#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define MSGLEN 32

int main()
{
    int svrSock, cliSock;
    int svrLen, cliLen;
    struct sockaddr_un svrAddr;
    struct sockaddr_un cliAddr;
    char readbuf[MSGLEN], sendbuf[MSGLEN];

    unlink("newSocket");
    svrSock = socket(AF_UNIX, SOCK_STREAM, 0);
    svrAddr.sun_family = AF_UNIX;
    strcpy(svrAddr.sun_path, "newSocket");
    svrLen = sizeof(svrAddr);

    if (bind(svrSock, (struct sockaddr *)&svrAddr, svrLen) < 0)
    {
        fprintf(stderr, "Binding error!\n");
        exit(1);
    }

    if (listen(svrSock, 5) < 0)
    {
        fprintf(stderr, "Listening error!\n");
        exit(1);
    }

    while (1)
    {
        cliLen = sizeof(cliAddr);
        printf("Waiting for client...\n");
        cliSock = accept(svrSock, (struct sockaddr *)&cliAddr, &cliLen);
        printf("Client accepted!\n");

        if (cliSock < 0)
        {
            fprintf(stderr, "Accept failed!\n");
            exit(1);
        }

        printf("Waiting for client sending...\n");
        read(cliSock, readbuf, MSGLEN);
        printf("Recved Msg: %s\n", readbuf);

        sprintf(sendbuf, "<Echo> %s", readbuf);
        write(cliSock, sendbuf, MSGLEN);
        close(cliSock);

        if (!strncmp(sendbuf, "quit", 4))
        {
            printf("Terminate Msg Recved!\n");
            break;
        }
    }

    return 0;
}