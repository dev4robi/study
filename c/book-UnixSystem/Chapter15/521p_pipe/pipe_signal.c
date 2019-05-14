#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>

#define MAXBUF 64

int sigpipeHandler()
{
    printf("\n파이프가 비정상적으로 닫혔습니다.\n");
    printf("SIGPIPE 핸들러 호출, 작업 종료중...\n");
    sleep(1);
    printf("\n<<< 작업 종료 ... >>>\n");
    exit(1);
}

int main(int argc, char **argv)
{
    char putMsg[MAXBUF], getMsg[MAXBUF];
    int pipeDes[2];
    int childPid = 0;

    printf("SIGPIPE 핸들러 세팅.\n\n");
    signal(SIGPIPE, (void*)sigpipeHandler);

    if (pipe(pipeDes) == -1)
    {
        printf("파이프 생성 실패.\n");
        return 0;
    }

    if ((childPid = fork()) == 0)
    {
        printf("<<< 자식 프로세스 >>>\n");

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
    else if (childPid > 0)
    {
        printf("<<< 부모 프로세스 >>>\n");

        while (1)
        {
            printf("INPUT PIPE : ");
            fgets(putMsg, MAXBUF, stdin);

            if (!strncmp(putMsg, "quit", 4))
            {
                close(pipeDes[0]);
                write(pipeDes[1], putMsg, MAXBUF);
                sleep(1);
            }

            write(pipeDes[1], putMsg, MAXBUF);
            sleep(1);
        }
    }
    else
    {
        printf("프로세스 생성에 실패했습니다.\n");
    }

    return 0;
}