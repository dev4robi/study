#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    int sockFd, addrLen;
    struct sockaddr_in sockAddr;
    char **alias, **addrList;
    struct hostent *hostT;
    struct servent *servT;
    char readBuf[128];
    int readCnt;

    if (argc == 2)
    {
        hostT = (struct hostent*)gethostbyname(argv[1]);
    }
    else
    {
        hostT = (struct hostent*)gethostbyname("localhost");
    }

    if (hostT == NULL)
    {
        fprintf(stderr, "Fail to get host info.\n");
        exit(1);
    }

    fprintf(stdout, "\n\n<< HOST:%s >>\n", hostT->h_name);
    alias = hostT->h_aliases;
    readCnt = 1;

    while (*alias)
    {
        fprintf(stdout, "host alias[%d]: %s\n", readCnt, *alias);
        ++alias;
        ++readCnt;
    }

    addrList = hostT->h_addr_list;
    readCnt = 1;

    while (*addrList)
    {
        fprintf(stdout, "IP Addr[%d]: %s\n", readCnt, inet_ntoa(*(struct in_addr *)*addrList));
        ++addrList;
        ++readCnt;
    }

    servT = (struct servent*)getservbyname("daytime", "tcp");

    if (servT == NULL)
    {
        fprintf(stderr, "Cannot use daytime service.\n");
        exit(1);
    }

    fprintf(stdout, "DAYTIME Service port number<TCP> : %d\n", ntohs(servT->s_port));

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = servT->s_port;
    sockAddr.sin_addr = *(struct in_addr *)*hostT->h_addr_list;
    addrLen = sizeof(sockAddr);

    if (connect(sockFd, (struct sockaddr *)&sockAddr, addrLen) < 0)
    {
        fprintf(stderr, "Fail to connect.\n");
        exit(1);
    }

    readCnt = read(sockFd, readBuf, sizeof(readBuf));
    readBuf[readCnt] = '\0';
    fprintf(stdout, "Daytime : %s\n", readBuf);
    close(sockFd);
    exit(0);
}