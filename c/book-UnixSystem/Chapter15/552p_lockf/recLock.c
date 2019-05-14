#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int file, step;

    for (step = 0; step < 5; ++step)
    {
        file = open("./db.txt", O_CREAT | O_WRONLY | O_APPEND);

        if (file == -1)
        {
            fprintf(stderr, "파일 열기 실패.\n");
            exit(0);
        }
    
        // 우분투 환경에서 좀 이상하게 제공된다.... 차라리 chmod나 fcntl를 사용해서 락 걸어버리는것도...?

        lockf(file, F_RDLCK, 0L);               // 읽기 락
        write(file, "recLock Message\n", 16);

        lockf(file, F_UNLCK, 0L);               // 락 해제
        close(file);
        sleep(1);
    }

    exit(1);
}