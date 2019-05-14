#include <stdio.h>
#include <pthread.h>
#include <sys/unistd.h>

pthread_mutex_t thread1Mx = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t thread2Mx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  checker   = PTHREAD_COND_INITIALIZER;

int countNo = 0;

void setCount(void)
{
    countNo++;
    sleep(1);
}

void *runTh1(void *arg)
{
    printf("첫 번째 스레드 실행\n");

    while (1)
    {
        if (countNo >= 5) pthread_exit(0);

        pthread_mutex_lock(&thread1Mx); ///// th1 mutex lock
        setCount();
        printf("첫 번째 스레드가 얻어온 카운트 번호 : %d\n", countNo);
        pthread_cond_signal(&checker);
        pthread_cond_wait(&checker, &thread1Mx);
        pthread_mutex_unlock(&thread1Mx); ///// th1 mutex unlock
    }
}

void *runTh2(void *arg)
{
    printf("두 번째 스레드 실행\n");

    while (1)
    {
        if (countNo >= 5) pthread_exit(0);

        pthread_mutex_lock(&thread2Mx); ///// th2 mutex lock
        pthread_cond_wait(&checker, &thread2Mx);
        setCount();
        printf("두 번째 스레드가 얻어온 카운트 번호 : %d\n", countNo);
        pthread_cond_signal(&checker);
        pthread_mutex_unlock(&thread2Mx); ///// th2 mutex unlock
    }
}

int main()
{
    pthread_t t1, t2;

    if (pthread_create(&t1, NULL, runTh1, NULL))
    {
        printf("th1 failed\n");
        return 0;
    }

    if (pthread_create(&t2, NULL, runTh2, NULL))
    {
        printf("th2 failed\n");
        return 0;
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&thread1Mx);
    pthread_mutex_destroy(&thread2Mx);
    pthread_cond_destroy(&checker);

    return 1;
}