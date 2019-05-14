#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/unistd.h>

#define COMMANDSIZ 64

int main()
{
    void *s_memory = NULL;
    int shmId;
    char *buffer;
    int isRun = 1;

    // shmget을 이용하여 공유 메모리 확보
    if ((shmId = shmget((key_t)9000, COMMANDSIZ, 0666 | IPC_CREAT)) == -1)
    {
        fprintf(stderr, "shmget() 생성 실패.\n");
        return 0;
    }

    // shmat을 이용하여 공유 메모리 주소 얻기
    if ((s_memory = shmat(shmId, NULL, 0)) == (void *)-1)
    {
        fprintf(stderr, "shmat() 실행 실패.\n");
        return 0;
    }

    // 공유 메모리 주소와 내부 변수 포인터 연결
    buffer = (char *)s_memory;

    while (isRun)
    {
        // ON 이면 상대방이 가져갈 때 까지 대기
        while (strncmp(buffer, "ON", 2) == 0) { usleep(100); }

        // 공유 메모리에 명령 라인 입력
        fprintf(stdout, "명령 입력(Max 62byte): ");
        fgets(buffer + 2, 62, stdin);
        strncpy(buffer, "ON", 2);

        // ON 문자열 뒤에 quit이 입력되었으면 종료
        if (!strncmp(buffer + 2, "quit", 4))
        {
            isRun = 0;
        }
    }

    if (shmdt(s_memory) == -1)
    {
        fprintf(stderr, "shmdt() 실행 실패.");
        return 0;
    }

    return 1;
}