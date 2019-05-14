// http://coffeenix.net/doc/develop/sema.txt
// 좋은 정보가 담긴 사이트.

#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <semaphore.h>

const char *P_FILENAME = "./log.txt";
const char *P_FILEMODE = "a+";

int work()
{
    __pid_t pid = getpid(), sid = semget((key_t)1234, 1, 0666 | IPC_CREAT);
    int counter = 50;
    FILE *fp = NULL;
    struct sembuf semBuf = { sid, 0, SEM_UNDO };

    while (counter)
    {
        char buf[255];
        
        semBuf.sem_op = -1;
        if (semop(sid, &semBuf, 1) == -1) // when semop zero: blocking ( - semop )
        {
            fprintf(stderr, "semop(-) error!\n");
            --counter;
            continue;
        }

        fp = fopen(P_FILENAME, P_FILEMODE);
        sprintf(buf, "write() - PID:%d / CNT:%d", pid, counter);
        fprintf(fp, "%s\n", buf);
        fprintf(stdout, "%s\n", buf);
        fclose(fp);
        --counter;

        semBuf.sem_op = 1;
        if (semop(sid, &semBuf, 1) == -1) // ( + semop )
        {
            fprintf(stderr, "semop(+) error!\n");
        }

        usleep(1);
    }

    return 0;   
}

int main(int argc, char **argv)
{
    int pid, semid, rt;
    struct sembuf semBuf;

    // Semaphore
    semBuf.sem_flg = SEM_UNDO;
    semBuf.sem_num = 0;

    semid = semget((key_t)1234, 1, 0666 | IPC_CREAT);

    if (semctl(semid, 0, SETVAL, 1) == -1)
    {
        fprintf(stderr, "Fail to semctl()\n");
        return -1;
    }

    fprintf(stdout, "<<< semget : %d >>>\n", semid);

    // Fork
    if ((pid = fork() > 0))
    {
        // Parent Process
        fprintf(stdout, "<<< Start Parent Process : PPID:%d/PID:%d >>>\n", getppid(), getpid());
        rt  = work();
        fprintf(stdout, "<<< End Parent Process : PPID:%d/PID:%d >>>\n", getppid(), getpid());
        return rt;
    }
    else if (pid == 0)
    {
        // Child Process
        fprintf(stdout, "<<< Start Child Process : PPID:%d/PID:%d >>>\n", getppid(), getpid());
        rt = work();
        fprintf(stdout, "<<< End Child Process : PPID:%d/PID:%d >>>\n", getppid(), getpid());
        return rt;
    }
    else
    {
        fprintf(stderr, "Fail to fork().\n");
        return -1;
    }
}