#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/wait.h>

int main()
{
    int childPID = 0;
    int count = 0;

    childPID = fork();

    if (childPID == 0)
    {
        printf("<< 자식 프로세스 생성 >>\n");

        for (count = 0; count < 2; ++count)
        {
            printf("CHILD PROCESS - count : %d\n", count);
            sleep(1);
        }
    }
    else if (childPID > 0)
    {
        printf("종료된 자식 프로세스: %d\n", wait((int*)0));
        printf("<< 부모 - 자식 프로세스 번호 : %d >>\n", childPID);

        for (count = 0; count < 2; ++count)
        {
            printf("PARENT PROCESS - count :%d\n", count);
            sleep(1);
        }
    }
    else
    {
        printf("프로세스 생성에 실패했습니다.\n");
    }

    return 0;
}