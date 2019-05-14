#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/unistd.h>

#define MAXBUF 64

int main()
{
    char putMsg[MAXBUF], getMsg[MAXBUF];
    int pipeDes[2];

    if (pipe(pipeDes) == -1)
    {
        printf("파이프 생성 실패.\n");
        return 0;
    }

    fcntl(pipeDes[0], F_SETFL, O_NDELAY); // 논블록 파이프

    sprintf(putMsg, "파이프에 메시지 입출력.");
    printf("INPUT PIPE : %s\n", putMsg);
    write(pipeDes[1], putMsg, MAXBUF);

    read(pipeDes[0], getMsg, MAXBUF);
    printf("GET MSG : %s\n", getMsg);

    return 1;
}