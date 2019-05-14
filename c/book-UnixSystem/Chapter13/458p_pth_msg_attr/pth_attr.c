#include <stdio.h>
#include <pthread.h>
#include <sys/unistd.h>

pthread_attr_t subAttr;

void* subThread(void *arg)
{
    size_t memSize;

    pthread_attr_getstacksize(&subAttr, &memSize);
    printf("서브 스레드 attr의 스택사이즈 : %ld\n", memSize);
    pthread_exit(0);
}

int main()
{
    pthread_t sth;
    size_t memSize;

    pthread_attr_init(&subAttr);
    pthread_attr_getstacksize(&subAttr, &memSize);
    printf("attr의 초기 스택사이즈 : %ld\n", memSize);

    printf("rst:%d\n", pthread_attr_setstacksize(&subAttr, 1024 * 3));
    pthread_attr_getstacksize(&subAttr, &memSize);
    printf("메인 스레드 attr의 스택사이즈 : %ld\n", memSize);

    if (pthread_create(&sth, &subAttr, subThread, NULL))
    {
        printf("서브스레드 생성 실패.\n");
        return 0;
    }

    pthread_join(sth, NULL);
    pthread_attr_destroy(&subAttr);
    return 1;
}