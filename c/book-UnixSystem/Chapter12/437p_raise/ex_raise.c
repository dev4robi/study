#include <sys/signal.h>
#include <sys/unistd.h>
#include <stdio.h>
#include <stdlib.h>

int commonStop()
{
    printf("\n\n시그널 핸들러 호출\n");
    sleep(1);
    printf("\n<<< 작업 종료 진행 >>>\n\n");
    exit(1);
}

int main(int argc, char **argv)
{
    int secs, steps = 1;

    if (argc == 2)
    {
        secs = atoi(argv[1]);

        signal(SIGTERM, (void *)commonStop);

        while (steps <= secs)
        {
            sleep(1);
            printf("%d번째 작업처리 중\n", steps);
            ++steps;
        }

        raise(SIGTERM);
    }
    else
    {
        printf("\n\n Usage: ex_raise no_of_secs\n\n\n");
    }
    
    return 1;
}