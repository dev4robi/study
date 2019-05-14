#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/wait.h>

void forChild()
{
    printf("<< 자식 프로세스 ps로 대체 >>\n");
    execl("/bin/ps", "ps", "-f", (char *)0);

    printf("execl() 실행 오류.\n");
    exit(1);
}

int main(int argc, char **argv)
{
    int childPID = 0;
    int parentCnt = 0;

    childPID = fork();

    if (childPID == 0)
    {
        printf("<< 자식 프로세스 생성 >>\n");
        forChild();
    }
    else if (childPID > 0)
    {
        printf("<< 부모 - 자식 프로세스 번호 : %d >>\n", childPID);

        for (parentCnt = 0; parentCnt < 2; ++parentCnt)
        {
            printf("PARENT PROCESS - count : %d\n", parentCnt);
            sleep(1);
        }
    }
    else
    {
        printf("프로세스 생성에 실패했습니다.\n");
    }

    return 0;
}