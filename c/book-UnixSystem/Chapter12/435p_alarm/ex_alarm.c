#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <sys/signal.h>

int defaultVal;

int alaramHandler(int arg)
{
    printf("\n시간이 다 되었습니다.\n");
    defaultVal = -1;
    return 1;
}

int main()
{
    signal(SIGALRM, (void *)alaramHandler);
    alarm(3);

    printf("3초 안에 입력해 주세요. DEFAULT VALUE: ");
    scanf("%d", &defaultVal);

    alarm(0);

    printf("DEFAULT VALUE: %d\n", defaultVal);
    return 1;
}