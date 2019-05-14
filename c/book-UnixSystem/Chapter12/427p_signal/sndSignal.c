#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>

int main(int argc, char **argv)
{
    int pid;

    if (argc != 2)
    {
        printf("\n\n Usage: sndSigal processID\n\n\n");
        exit(0);
    }

    pid = atoi(argv[1]);
    kill(pid, SIGTERM);
    exit(1);
}