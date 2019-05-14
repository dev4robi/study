#include <stdio.h>
#include <sys/fcntl.h>      // creat()
#include <sys/types.h>
#include <sys/stat.h>       // mkdir()
#include <sys/unistd.h>     // close() chdir()

int main(int argc, char **argv)
{
    int accessMode, fd;
    char *dirPath = "./tempDir";
    char *filename = "./tempFile";

    if (mkdir(dirPath, 0777) < 0)
    {
        printf("mkdir() failed!\n");
        return -1;
    }

    if (chdir(dirPath) < 0)
    {
        printf("chdir() failed!\n");
        return -1;
    }

    if ((fd = creat(filename, 0644)) < 0)
    {
        printf("creat() failed!\n");
        return -1;
    }

    char yn;
    printf("폴더를 지우시겠습니까? (y/n) : ");
    scanf("%c", &yn);

    if (yn == 'y')
    {
        char cwdbuf[256];

        chdir("../");
        if (getcwd(cwdbuf, sizeof(cwdbuf)) != NULL)
        {
            printf("현재 경로:%s\n", cwdbuf);
        }

        if (rmdir(dirPath) < 0)
        {
            printf("rmdir() failed!\n");
            return -1;
        }
    }

    close(fd);
    return 0;
}