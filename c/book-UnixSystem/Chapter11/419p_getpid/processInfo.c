#include <stdio.h>
#include <sys/unistd.h> // getpid(), getuid(), getgid()

int main()
{
    int psPID, psUID, psGID;

    psPID = getpid();
    psUID = getuid();
    psGID = getgid();

    printf("<< 프로세스 정보 >>\n");
    printf("PID: %d, UID :%d, GID: %d\n", psPID, psUID, psGID);
    return 0;
}