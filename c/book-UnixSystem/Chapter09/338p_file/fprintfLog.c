#include <stdio.h>

int main(int argc, char **argv)
{
    char buf[256];
    FILE *file;
    file = fopen("./log.txt", "w");

    fprintf(stdout, "main() 함수가 실행되었습니다.\n");
    fprintf(file, "로그파일에 첫번째 로그를 남깁니다.");
    fprintf(file, "로그파일에 두번째 로그를 남깁니다.");
    fprintf(stderr, "두번째 로그는 에러입니다.\n");
    fclose(file);
    fprintf(stdin, "이러면 어떨까?");                   // 암것도 안된다?
    fread(buf, sizeof(buf), sizeof(buf), stdin);
    fflush(stdin);
    fprintf(stdout, "이러면? : %s\n", buf);

    return 0;
}