#include <stdio.h>      // printf()
#include <string.h>     // memset()
#include <sys/stat.h>   // chmod(), stat(), struct stat
#include <fcntl.h>      // open(), creat(), fcntl()
#include <unistd.h>     // close()

int main(int argc, char **argv)
{
    int accessMode = 0777;
    int result, fd;
    char *filename = "./Temp.txt";

    // 파일 생성
    fd = creat(filename, 0644);

    if (fd < 0)
    {
        printf("File create error!\n");
        return -1;
    }

    // 파일 모드 변경
    if (chmod(filename, accessMode) < 0)
    {
        close(fd);
        printf("File mode change error!\n");
        return -1;
    }

    // 파일 열기
    fd = open(filename, O_RDONLY | O_TRUNC);

    if (fd < 0)
    {
        printf("File open error!\n");
        return -1;
    }

    // 열린파일 플래그 변경 
    fcntl(fd, O_RDWR | O_APPEND);

    // 파일정보 획득및 출력
    struct stat sb;
    
    memset(&sb, 0, sizeof(sb));
    result = stat(filename, &sb);

    printf("StatResult:%d\n", result);
    printf("dev_t:%lu, ino_t:%lu, st_mode:%hd, st_nlink:%lu, st_uid:%hd, st_gid:%hd,\n"\
           "st_rdev:%lu, st_size:%ld, st_atime:%ld, st_mtime:%ld, st_ctime:%ld\n\n",
            sb.st_dev, sb.st_ino, sb.st_mode, sb.st_nlink, sb.st_uid, sb.st_gid,
            sb.st_rdev, sb.st_size, sb.st_atime, sb.st_mtime, sb.st_ctime);
    printf("st_dev: 파일이 포함된 논리적 장치\nst_size: 파일의 바이트 수\n"\
           "st_ino: i-node 번호\nst_atime: 마지막으로 읽힌 시간\n"\
           "st_nlink: 파일의 링크 개수\nst_mtime: 갱신된 시간\n"\
           "st_uid/gid: 사용자/그룹 번호\nst_ctime: 파일 구조자체의 변경\n");

    // 파일 닫고 종료
    result = close(fd);
    return 0;
}