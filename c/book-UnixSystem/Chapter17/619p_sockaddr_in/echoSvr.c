#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MSGLEN 32

int main()
{
    int svrSock, cliSock;
    int svrLen, cliLen;
    struct sockaddr_in svrAddr;
    struct sockaddr_in cliAddr;
    char readbuf[MSGLEN], sendbuf[MSGLEN];

    if ((svrSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "socket() failed!\n");
        exit(1);
    }

    svrAddr.sin_family = AF_INET;
    svrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    svrAddr.sin_port = 9999;

    svrLen = sizeof(svrAddr);

    if (bind(svrSock, (struct sockaddr *)&svrAddr, svrLen) < 0)
    {
        fprintf(stderr, "Binding failed!\n");
        exit(1);
    }

    if (listen(svrSock, 5) < 0)
    {
        fprintf(stderr, "Listen failed!\n");
        exit(1);
    }

    int val;
    if ((val = fcntl(cliSock, F_GETFL, 0)) < 0)
    {
        fprintf(stderr, "fcntl() error!\n");
        exit(1);
    }
    else
    {
        val = !(val & O_NONBLOCK);
        fprintf(stdout, "rst:%d\n", val);
    }

    while (1)
    {
        cliLen = sizeof(cliAddr);
        cliSock = accept(svrSock, (struct sockaddr *)&cliAddr, &cliLen);

        if (cliSock < 0)
        {
            fprintf(stderr, "Accept() failed! (rt:%d)\n", cliSock);
            exit(1);
        }

        printf("Waiting for client msg...");
        read(cliSock, readbuf, MSGLEN);
        printf("Recv: %s\n", readbuf);

        sprintf(sendbuf, "<Echo> %s", readbuf);
        write(cliSock, sendbuf, MSGLEN);
        close(cliSock);
    }

    return 0;
}