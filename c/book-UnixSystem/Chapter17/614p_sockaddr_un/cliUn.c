#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // read(), write()
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define MSGLEN 32

int main()
{
    int sockFd, addrLen;
    struct sockaddr_un sockAddr;
    char msgbuf[MSGLEN];

    sockFd = socket(AF_UNIX, SOCK_STREAM, 0);
    sockAddr.sun_family = AF_UNIX;
    strcpy(sockAddr.sun_path, "newSocket");
    addrLen = sizeof(sockAddr);

    if (connect(sockFd, (struct sockaddr *)&sockAddr, addrLen) == -1)
    {
        fprintf(stderr, "Fail to connect server!\n");
        exit(1);
    }

    printf("MSG: ");
    memset(msgbuf, 0x00, MSGLEN);
    fgets(msgbuf, MSGLEN - 1, stdin);

    write(sockFd, msgbuf, MSGLEN);
    read(sockFd, msgbuf, MSGLEN);

    printf("Echo: %s\n", msgbuf);
    close(sockFd);
    exit(0);
}