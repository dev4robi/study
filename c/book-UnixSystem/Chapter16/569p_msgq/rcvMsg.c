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

    // 메시지 큐 ID 얻어오기
    if ((msgQid = msgget((key_t)8000, 0666 | IPC_CREAT)) == -1)
    {
        fprintf(stderr, "msgget() 실행 실패. (errno:%d)\n", errno);
        return 0;
    }

    // quit 을 입력받을 때 까지 반복
    while (isRun)
    {
        // 메시지 읽기
        if ((result = msgrcv(msgQid, (void *)&ut, BUFLEN, msgType, 0)) == -1) // msgType 에 해당하는 메시지만 큐에서 가져옴
        {
            fprintf(stderr, "msgrcv 실행 실패. (errno:%d)\n", errno);
            return 0;
        }

        // 읽어들인 메시지 출력, quit 여부 체크
        fprintf(stdout, "<< 사용자 >>\n주민번호: %s\n", ut.userNo);
        fprintf(stdout, "주소: %s\n", ut.address);
        
        if (!strncmp(ut.userNo, "quit", 4))
        {
            isRun = 0;
        }
    }

    // 메시지 큐 제거
    if ((result = msgctl(msgQid, IPC_RMID, 0)) == -1)
    {
        fprintf(stderr, "msgctl 실행 실패.\n");
        return 0;
    }

    return 1;
}