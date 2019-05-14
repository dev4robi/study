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
    int svrSock, cltSock;
    int svrLen, cltLen;
    struct sockaddr_in svrAddr, cltAddr;
    char readbuf[MSGLEN], sendbuf[MSGLEN];

    svrSock = socket(AF_INET, SOCK_STREAM, 0);

    if (svrSock < 0)
    {
        fprintf(stderr, "Socket open error.\n");
        exit(1);
    }

    svrAddr.sin_family = AF_INET;
    svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    svrAddr.sin_port = 9999;
    svrLen = sizeof(svrAddr);

    if (bind(svrSock, (struct sockaddr *)&svrAddr, svrLen) < 0)
    {
        fprintf(stderr, "Binding error.\n");
        exit(1);
    }

    if (listen(svrSock, 5) < 0)
    {
        fprintf(stderr, "Listen error.\n");
        exit(1);
    }

    cltLen = sizeof(cltAddr);
    cltSock = accept(svrSock, (struct sockaddr *)&cltAddr, &cltLen);

    if (cltSock < 0)
    {
        fprintf(stderr, "Accept error.\n");
        exit(1);
    }

    while (1)
    {
        fprintf(stdout, "Waiting for client msg...\n");
        read(cltSock, readbuf, MSGLEN);
        fprintf(stdout, "Recved msg: %s\n", readbuf);

        if (!strncmp(readbuf, "quit", 4)) break;

        // sleep(20); // palnned timeout for test!

        sprintf(sendbuf, "<Echo> %s", readbuf);
        write(cltSock, sendbuf, MSGLEN);
    }

    close(cltSock);
    close(svrSock);
    exit(0);
}