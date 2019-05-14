#include <stdio.h>
#include <sys/wait.h>

int main()
{
    while (1)
    {
        fprintf(stderr, "ONLYONE PROCESS RUNNING\n");
        sleep(1);
    }

    return 1;
}