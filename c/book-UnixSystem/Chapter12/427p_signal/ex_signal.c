/*
    [ 주로 사용되는 시그널 ]
     1. SIGHUP  : Hangup을 위한 시그널로 터미널과 시스템 사이에 통신 접속이 끊어졌을 때, 터미널에 연결된 프로세스들에게 커널이 보내는 시그널.
     2. SIGINT  : Interrupt를 위한 시그널로 유저가 인터럽트를 발생시키는 키를 입력했을 때 그와 연결된 프로세스에게 커널이 보내는 시그널. 프로세스를 종료할 때 많이 사용.
     3. SIGQUIT : Quit을 위한 시그널로 유저가 터미널에서 Quit 키를 치면 커널이 프로세스에게 SIGQUIT 시그널을 보냄.
     4. SIGILL  : Illegal 명령, 즉 비정상적인 명령을 수행할 때 OS가 발생시키는 시그널.
     5. SIGTRAP : Trace Trap을 위한 시그너로 디버거들이 주로 사용하는 시그널.
     6. SIGABRT : Abort 실행 시 발생하는 시그널로 Abort는 시스템이 비정상적으로 종료될 때 해당 정보를 남기는 명령.
     7. SIGIOT  : SIGABRT와 유사한 작업을 수행할 때 발생하는 시그널.
     8. SIGEMT  : Emt 명령 실행 시 사용되는 시그널.
     9. SIGFPE  : FLoating 포인터 예외 상황, 즉 부동소숫점 사용에서 오버플로우나 언더플로우가 발생했을 때 사용되는 시그널.
    10. SIGKILL : 프로세스가 다른 프로세스를 Kill 시키기 위해 발생하는 시그널.
    11. SIGBUS  : Bus 에러가 발생했을 때 사용되는 시그널.
    12. SIGSEGV : 메모리 세그먼트등이 꺠졌을 때 발생하는 시그널.
    13. SIGSYS  : 시스템 호출을 할 때 잘못된 인수를 사용하면 발생하는 시그널.
    14. SIGPIPE : 파이프에서 사용하는 시그널로 아무도 읽지 않는 파이프에 데이터 출력할 때 발생하는 시그널.
    15. SIGALRM : 알람 클락 시그널로 해당 타이머가 끝나면 발생하는 시그널.
    16. SIGTERM : Kill에 의해 프로세스가 종료할 때 발생되는 시그널.
    ...

    [Note] 5, 7, 8, 11, 13 은 POSIX.1 에 없는 시그널.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/unistd.h>

void sigintHandler(int arg)
{
    printf("\n\nSIGINT 핸들러 호출\n");
    printf("\n<<< 작업 종료 시작 >>>\n");
    sleep(1);
    printf("\n\nStop all run process\n");
    printf("All open file closed\n\n\n");
    exit(1);
}

void sigquitHandler(int arg)
{
    printf("\n\nSIGQUIT 핸들러 호출\n");
    printf("\n<<< 작업 종료 시작 >>>\n");
    sleep(1);
    printf("\n\nStop all run process\n");
    printf("All open file closed\n\n\n");
    exit(1);
}

int main()
{
    int step = 0;

    printf("SIGINT 핸들러 세팅\n\n");
    signal(SIGINT, sigintHandler);
    signal(SIGQUIT, sigquitHandler);

    printf("\n<<< Main 프로세스 실행 >>>\n");
    printf("File open 실행\n");

    while (1)
    {
        ++step;
        printf("%d번째 작업수행\n", step);
        sleep(1);
    }

    return 1;
}