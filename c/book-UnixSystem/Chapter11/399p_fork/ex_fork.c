#include <stdio.h>
#include <sys/unistd.h>
#include <sys/wait.h>

#define FOREVER ;;

void forParent()
{
    int parentCnt = 0;

    for (FOREVER)
    {
        printf("PRINT PROCESS - count : %d\n", parentCnt);
        ++parentCnt;
        sleep(3);
    }
}

void forChild()
{
    int childCnt = 0;

    for (FOREVER)
    {
        printf("CHILD PROCESS - count : %d\n", childCnt);
        ++childCnt;
        sleep(5);
    }
}

int main(int argc, char **argv)
{
    int childPID = 0;

    childPID = fork();

    if (childPID == 0)
    {
        printf("<<자식 프로세스 생성>>\n");
        forChild();
    }
    else if (childPID > 0)
    {
        printf("<< 부모 - 자식 프로세스 번호: %d >>\n", childPID);
        forParent();
    }
    else
    {  
        printf("프로세스 생성 실패.\n");
    }

    return 0;
}