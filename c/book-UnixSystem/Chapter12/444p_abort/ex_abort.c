#include <sys/signal.h>
#include <sys/unistd.h>
#include <stdio.h>
#include <stdlib.h>

int sigHandler(int arg)
{
    printf("\n\nsigHandler 핸들러 호출\n");
    abort();
}

int main()
{
    printf("시그널 핸들러 세팅\n\n");
    signal(SIGINT, (void *)sigHandler);

    printf("\n<<< Main 프로세스 실행 >>>\n");

    while (1)
    {
        printf("MAIN 작업수행\n");
        sleep(1);
    }

    return 1;
}