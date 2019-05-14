#include <stdio.h>
#include <pthread.h>
#include <sys/unistd.h>

typedef struct
{
    char *ipAddr;
    char *hostName;
    int portNo;
} IpInfoType;

int countNo = 0;

IpInfoType* get_ipInfo(void)
{
    static IpInfoType ipInfo;
    int mutexRlt;

    countNo++;
    sleep(3);

    ipInfo.ipAddr = "192.168.8.100";
    ipInfo.hostName = "JSHIN";
    ipInfo.portNo = countNo;

    return &ipInfo;
}

void* setConnect(void *arg)
{
    IpInfoType *subInfo;
    printf("SETCONNECT 스레드 실행\n");

    while (1)
    {
        subInfo = get_ipInfo();
        printf("서브 스레드가 가진 포트 번호: %d\n", subInfo->portNo);
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    IpInfoType *mainInfo;

    if (pthread_create(&tid, NULL, setConnect, NULL))
    {
        printf("tid 스레드 생성 실패.\n");
        return 0;
    }

    while (1)
    {
        mainInfo = get_ipInfo();
        printf("메인 스레드가 가진 포트 번호: %d\n", mainInfo->portNo);
        sleep(1);
    }
}