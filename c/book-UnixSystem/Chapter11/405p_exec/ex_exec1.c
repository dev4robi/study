#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>

int main()
{
    printf("ps -ef 를 실행합니다.\n");

    // 1. execl()
    //execl("/bin/ps", "ps", "-ef", (char *)0);
    
    // 2. execv()
    char *argv[3] = { "ps", "-ef", 0 };
    execv("/bin/ps", argv);

    printf("오류 발생!\n");

    exit(1);

    return 0;
}