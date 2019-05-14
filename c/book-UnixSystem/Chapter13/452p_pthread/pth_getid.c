#include <stdio.h>
#include <pthread.h>
#include <sys/unistd.h>

void* subThread(void *arg)
{
    pthread_t stid = pthread_self();

    printf("서브 스레드 id: %ld\n", stid);

    while (1)
    {
        sleep(1);
        printf("서브스레드 동작중...\n");
    }
}

int main()
{
    pthread_t st;
    pthread_t mtid = pthread_self();

    printf("메인 스레드 id: %ld\n", mtid);

    if (pthread_create(&st, NULL, subThread, NULL))
    {
        printf("서브 스레드 생성 실패...\n");
        return 0;
    }

    printf("메인 스레드 종료. 서브 스레드 id: %ld\n", st);
    pthread_exit(0);
}