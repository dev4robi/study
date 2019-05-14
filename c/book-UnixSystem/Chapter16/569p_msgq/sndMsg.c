#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUFLEN 32

typedef struct
{
    long int    msgType;
    char        userNo[13 + 1];
    char        address[17 + 1];
} UserType;

int main()
{
    int isRun = 1;
    int msgQid, result;
    UserType ut;
    long int msgType = 1;

    if ((msgQid = msgget((key_t)8000, IPC_CREAT)) == -1)
    {
        fprintf(stderr, "msgget() 실패. (errno:%d)\n", errno);
        return 0;
    }

    while (isRun)
    {
        memset(&ut, 0x00, BUFLEN);
        ut.msgType = 1;
        fprintf(stdout, "주민번호: ");
        fgets(ut.userNo, sizeof(ut.userNo) - 1, stdin);
        fprintf(stdout, "주소: ");
        fgets(ut.address, sizeof(ut.address) - 1, stdin);
        
        if ((result = msgsnd(msgQid, (void *)&ut, BUFLEN, 0)) == -1)
        {
            fprintf(stderr, "msgsnd() 실패.\n");
            isRun = 0;
        }

        if (!strncmp(ut.userNo, "quit", 4))
        {
            isRun = 0;
        }
    }

    return 1;
}