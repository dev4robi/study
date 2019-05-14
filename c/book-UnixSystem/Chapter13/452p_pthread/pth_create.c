#include <stdio.h>
#include <pthread.h>
#include <sys/unistd.h>

void* subThread(void *arg)
{
    while (1)
    {
        sleep(2);
        printf("서브 스레드 동작중!\n");
    }
}

int main()
{
    pthread_t tid;

    if (pthread_create(&tid, NULL, subThread, NULL))
    {
        printf("서브 스레드 생성 실패.\n");
        return 0;
    }

    while (1)
    {
        sleep(1);
        printf("메인 스레드 동작중!\n");
    }

    return 1;
}