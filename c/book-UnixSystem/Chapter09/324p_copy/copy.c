#include <stdio.h>
#include <string.h>
#include <sys/stat.h>   // chmod()
#include <fcntl.h>      // open(), creat()
#include <unistd.h>     // close(), link(), write(), read()

#define BUFLEN 256

int main(int argc, char **argv)
{
    int readCnt, writeCnt, oriFile, newFile;
    char buffer[BUFLEN];

    if (argc != 3)
    {
        printf("Usage: copy ori_filename new_filename \n\n");
        return -1;
    }

    // 원본 열기 및 복사본 생성/열기
    oriFile = open(argv[1], O_RDONLY);
    newFile = creat(argv[2], O_WRONLY | O_CREAT | O_APPEND);
    printf("oriFile: %d, newFile: %d\n", oriFile, newFile);

    if (oriFile < 0 || newFile < 0)
    {
        printf("File open failed!\n");
        return -1;
    }

    // 파일 복사
    for (readCnt = 1; readCnt > 0;)
    {
        memset(buffer, 0, BUFLEN);
        readCnt = read(oriFile, buffer, BUFLEN);
        writeCnt = write(newFile, buffer, BUFLEN);
        printf("readCnt: %d, writeCnt: %d\n", readCnt, writeCnt);
    }

    // 파일 속성 변경
    chmod(argv[2], 0666);

    // 파일 링크/언링크
    link(argv[2], "./test_link");
    unlink("./test_link");

    // 파일 닫고 종료
    close(oriFile);
    close(newFile);
    return 0;
}