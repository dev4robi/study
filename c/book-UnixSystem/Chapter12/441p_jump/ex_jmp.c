#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <sys/unistd.h>
#include <sys/signal.h>

jmp_buf jmpPos;
int     jmpCnt;

int runJump()
{
    ++jmpCnt;

    if (jmpCnt < 3)
    {
        printf("\n아직 종료할 수 없다! (%d)\n", jmpCnt);
    }
    else
    {
        printf("\n이번에는 종료모듈로 가자! (%d)\n", jmpCnt);
        
    }

    signal(SIGINT, SIG_IGN); // 이 시점에 발생하는 SIGINT 시그널 무시(SIG_IGN)
    longjmp(jmpPos, jmpCnt);
}

int main()
{
    int result = 0;

    result = setjmp(jmpPos);

    if (result >= 3)
    {
        printf("\n시스템 종료 중...\n");
        sleep(2);
        exit(1);
    }

    signal(SIGINT, (void *)runJump);

    while (1)
    {
        sleep(1);
        printf("%d번째 단계로 프로그램 실행 중... [강종해 보시지? CTRL-C]\n", result);
    }

    return 1;
}