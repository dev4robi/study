#include <stdio.h>
#include <time.h>

char* getDate()
{
    time_t curtime;
    struct tm *loctime;
    static char datedt[10 + 1];

    curtime = time(NULL);
    loctime = localtime(&curtime);
    strftime(datedt, 10 + 1, "%Y%m%d", loctime);

    ////////////////////////////////////////////

    static char datedt2[21 + 1];

    curtime = time(NULL);
    loctime = localtime(&curtime);
    strftime(datedt, 22, "%Y-%m-%d, %T", loctime);

    printf("curtime:%ld\n", curtime);
    
    return datedt;
}

int main()
{
    printf("DATE : %s\n\n", getDate());
    return 0;
}