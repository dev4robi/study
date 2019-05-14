#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/unistd.h>

int stopHandler(int arg)
{
    printf("\n\n시그널 핸들러 호출\n");
    sleep(1);
    printf("\n<<< 작업 종료 진행 >>>\n\n");
    exit(1);
}

int main()
{
    int step = 0;

    printf("SIGTERM 핸들러 세팅\n\n");
    signal(SIGTERM, (void*)stopHandler);

    printf("\n<<< Main 프로세스 실행, PID: %d >>>\n", getpid());
    printf("File open 실행\n");

    while (1)
    {
        ++step;
        printf("%d번째 작업 수행\n", step);
        sleep(1);
    }

    return 1;
}