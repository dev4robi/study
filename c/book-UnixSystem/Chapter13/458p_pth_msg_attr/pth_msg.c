#include <stdio.h>
#include <pthread.h>
#include <sys/unistd.h>

typedef struct
{
    char *ipAddr;
    char *hostName;
    int portNo;

} IpInfoType;

void *setConnect(void *ipInfo)
{
    IpInfoType *connInfo = (IpInfoType *)ipInfo;

    printf("SETCONNECT 스레드 실행");
    printf("IPADDRESS: %s\n", connInfo->ipAddr);
    printf("HOSTNAME: %s\n", connInfo->hostName);
    printf("PORTNO: %d\n", connInfo->portNo);
    printf("해당 시스템으로 접속 중...\n");
    sleep(1);
}

int main()
{
    pthread_t setConnect_t;
    
    IpInfoType ipInfo;
    ipInfo.ipAddr = "192.168.8100";
    ipInfo.hostName = "JSHIN";
    ipInfo.portNo = 9000;

    if (pthread_create(&setConnect_t, NULL, setConnect, (void *)&ipInfo))
    {
        printf("setConnect_t 스래드 생성 실패!\n");
        return 0;
    }

    pthread_exit(0);
}
