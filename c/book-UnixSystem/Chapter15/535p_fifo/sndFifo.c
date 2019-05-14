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

    if ((fd = open("FIFO", O_WRONLY)) < 0)
    {
        printf("FIFO 파이프 열기 실패.\n");
        return 0;
    }

    while (1)
    {
        printf("메시지 입력: ");
        fgets(buf, MAXBUF, stdin);

        if (write(fd, buf, MAXBUF) == -1)
        {
            printf("파이프에 메시지 쓰기 실패.\n");
            break;
        }
        if (!strncmp(buf, "quit", 4))
        {
            exit(0);
        }
    }

    return 0;
}