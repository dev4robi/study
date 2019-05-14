#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>

#define MAXBUF 64

int main()
{
    char putMsg[MAXBUF], getMsg[MAXBUF];
    int pipeDes[2];
    int childPID = 0;

    if (pipe(pipeDes) == -1)
    {
        printf("파이프 생성 실패.\n");
        return 0;
    }

    if ((childPID = fork()) == 0)
    {
        printf("<< 자식 프로세스 >>\n");

        while (1)
        {
            read(pipeDes[0], getMsg, MAXBUF);
            printf("GET MSG: %s\n", getMsg);

            if (!strncmp(getMsg, "quit", 4))
            {
                exit(1);
            }
        }
    }
    else if (childPID > 0)
    {
        printf("<< 부모 프로세스 >>");

        while (1)
        {
            printf("INPUT PIPE: ");
            fgets(putMsg, MAXBUF, stdin);
            write(pipeDes[1], putMsg, MAXBUF);

            if (!strncmp(putMsg, "quit", 4))
            {
                exit(1);
            }

            sleep(1);
        }
    }
    else
    {
        printf("자식 프로세스 생성 실패.\n");
    }

    return 0;
}