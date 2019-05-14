#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/stat.h> // mkfifo()
#include <sys/unistd.h>
#include <errno.h>

#define MAXBUF 64

int main()
{
    int fd;
    char buf[MAXBUF];

    if (mkfifo("FIFO", 0666) == -1)
    {
        if (errno != EEXIST)
        {
            printf("FIFO 파이프 생성 실패!\n");
            return -1;
        }
    }

    if ((fd = open("FIFO", O_RDWR)) < 0)
    {
        printf("FIFO 파이프 열기 실패.\n");
        return 0;
    }

    while (1)
    {
        if ((read(fd, buf, MAXBUF)) < 0)
        {
            printf("메시지 읽기 실패.\n");
            break;
        }
        else
        {
            printf("읽은 메시지 : %s\n", buf);

            if (!strncmp(buf, "quit", 4))
            {
                exit(0);
            }
        }
    }

    return 0;
}