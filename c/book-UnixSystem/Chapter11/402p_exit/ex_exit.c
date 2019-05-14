#include <stdio.h>
#include <stdlib.h>     // exit()
#include <sys/unistd.h> // fork()
#include <sys/wait.h>   // sleep()

int main(int argc, char **argv)
{
    int step = 0;
    int childPID = 0;

    childPID = fork();

    if (childPID == 0)
    {
        printf("<< 자식 프로세스 >>\n");

        for (step = 0; step < 10; ++step)
        {
            printf("자식 실행 횟수: %d\n", step);
            sleep(1);
        }

        printf("자식 종료.\n");
        exit(1);
    }
    else if (childPID > 0)
    {
        printf("<< 부모 프로세스 >>\n");

        for (step = 0; step < 15; ++step)
        {
            printf("부모 실행 횟수: %d\n", step);
            sleep(1);
        }

        printf("부모 종료.\n");
        exit (1);
    }
    else
    {
        printf("프로세스 생성 실패.\n");
    }

    return 0;
}