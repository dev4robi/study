#include <stdio.h>
#include <pthread.h>
#include <sys/unistd.h>

void* subThread(void *arg)
{
    printf("서브 스레드 시작.\n");
    sleep(1);
    printf("서브 스레드 완료.\n");
    pthread_exit((void *)1);
}

int main()
{
    pthread_t stid;
    int stsVal;

    if (pthread_create(&stid, NULL, subThread, NULL))
    {
        printf("서브 스레드 생성 실패...\n");
        return 0;
    }

    pthread_join(stid, (void **)&stsVal);
    printf("PSTSVAL: %d\n", stsVal);
    return 1;
}