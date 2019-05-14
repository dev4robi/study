#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MSG_LEN 32

int main(int argc, char **argv)
{
    int fpid;
    int svrSoc, cliSoc;
    int svrLen, cliLen;
    struct sockaddr_in svrAddr, cliAddr;
    char readbuf[MSG_LEN], sendbuf[MSG_LEN];

    if ((svrSoc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "socket() error.\n");
        exit(1);
    }

    svrAddr.sin_family = AF_INET;
    svrAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    svrAddr.sin_port = 9999;
    svrLen = sizeof(svrAddr);

    if (bind(svrSoc, (struct sockaddr *)&svrAddr, svrLen) < 0)
    {
        fprintf(stderr, "bind() error.\n");
        exit(1);
    }

    if (listen(svrSoc, 5) < 0)
    {
        fprintf(stderr, "listen() error.\n");
        exit(1);
    }

    signal(SIGCHLD, SIG_IGN);

    while (1)
    {
        cliLen = sizeof(cliAddr);
        
        if ((cliSoc = accept(svrSoc, (struct sockaddr *)&cliAddr, &cliLen)) < 0)
        {
            fprintf(stderr, "accept() error.\n");
            exit(1);
        }

        if ((fpid = fork()) < 0) // fork error
        {
            fprintf(stderr, "fork() error.\n");
            exit(1);
        }
        else if (fpid == 0) // child process
        {
            fprintf(stdout, "Waiting for msg from client...\n");
            read(cliSoc, readbuf, MSG_LEN);
            fprintf(stdout, "Recved Msg : %s\n", readbuf);

            sleep(5); // sleep for multi client connection test!

            sprintf(sendbuf, "<Echo> %s", readbuf);
            write(cliSoc, sendbuf, MSG_LEN);
            close(cliSoc);
            exit(0);
        }
        else // parent process
        {
            close(cliSoc);
        }
    }

    exit(1);
}