#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    int sockFd;
    struct sockaddr_in sockAddr;
    char readBuf[128];
    int readCnt;

    if (argc != 2)
    {
        fprintf(stderr, "\nUsage: testUDP hostname\n\n");
        exit(1);
    }

    sockFd = socket(AF_INET, SOCK_DGRAM, 0); // Create UDP Socket

    if (sockFd < 0)
    {
        fprintf(stderr, "Socket open error.\n");
        exit(1);
    }

    bzero((char *)&sockAddr, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(argv[1]);
    sockAddr.sin_port = htons(13);

    sendto(sockFd, readBuf, 128, 0, (struct sockaddr*)&sockAddr, sizeof(sockAddr));
    readCnt = recvfrom(sockFd, readBuf, 128, 0, NULL, NULL);

    readBuf[readCnt] = '\0';
    fprintf(stdout, "Host daytime : %s\n", readBuf);
    close(sockFd);
    exit(0);
}