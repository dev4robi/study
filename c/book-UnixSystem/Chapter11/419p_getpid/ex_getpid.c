#include <stdio.h>
#include <unistd.h>

int main()
{
    int childPID = 0;
    int parentPID = 0;

    parentPID = getpid();

    childPID = fork();

    if (getpid() != parentPID)
    {
        printf("<< 자식 프로세스 >>\n");
        printf("자식 - 자식 PID: %d 부모 PID: %d\n", getpid(), getppid());
    }
    else if (getpid() == parentPID)
    {
        printf("<< 부모 프로세스 >>\n");
        printf("부모 - 자식 PID: %d, 부모 PID: %d\n", childPID, parentPID);
    }

    return 0;
}