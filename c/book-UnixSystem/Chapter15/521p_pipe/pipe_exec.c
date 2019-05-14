#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>

int main()
{
    int pipeDes[2];
    int childPid = 0;
    char *ps[3]   = { "ps", "-ef", NULL };
    char *grep[3] = { "grep", "sys", NULL };

    if (pipe(pipeDes) == -1)
    {
        printf("파이프 생성 실패.\n");
        return 0;
    }

    if ((childPid = fork()) == 0)
    {
        printf("<< 자식 프로세스 : ps 실행 >>\n");
        dup2(pipeDes[1], 1); // stdout
        close(pipeDes[0]);
        close(pipeDes[1]);
        execvp(ps[0], ps);
        printf("ps 실행에 실패!\n");
        return 0;
    }
    else if (childPid > 0)
    {
        printf("<< 부모 프로세스 : ps 결과를 받아 grep 실행 >>\n");
        dup2(pipeDes[0], 0); // stdin
        close(pipeDes[0]);
        close(pipeDes[1]);
        execvp(grep[0], grep);
        printf("grep 실행에 실패!\n");
        return 0;
    }
    else
    {
        printf("프로세스 생성에 실패!\n");
        return 0;
    }

    return 1;
}