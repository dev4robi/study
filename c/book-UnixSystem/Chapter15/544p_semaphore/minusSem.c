#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main(int argc, char **argv)
{
    int step;
    int sid, pid;
    FILE *file;
    struct sembuf semBuf;

    semBuf.sem_flg = SEM_UNDO;
    semBuf.sem_num = 0;
    
    sid = semget((key_t)1234, 1, 0666 | IPC_CREAT);

    if (semctl(sid, 0, SETVAL, 1) == -1)
    {
        fprintf(stderr, "세마포어 초기화 실패.\n");
        exit(0);
    }

    printf("MINUS 세마포어의 PID : %d\n", getpid());

    for (step = 0; step < 5; ++step)
    {
        semBuf.sem_op = -1;
        if (semop(sid, &semBuf, 1) == -1)   // process blocked
        {
            fprintf(stderr, "세마포어 값 감소 실패.\n");
            exit(0);
        }

        pid = semctl(sid, 0, GETPID, 0);
        printf("세마포어를 변경한 마지막 PID : %d\n", pid);

        file = fopen("./db.txt", "a+");
        fprintf(file, "minusSem 프로세스 메시지 저장\n");
        fclose(file);

        sleep(1);
    }

    if (semctl(sid, 0, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "세마포어 제거 실패.\n");
        exit(0);
    }

    exit(1);
}
