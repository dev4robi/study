#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>

int main(int argc, char **argv)
{
    FILE *wf;
    char buf[256];

    if (argc != 2)
    {
        printf("Usage: fileWrite filename \n\n");
        return -1;
    }

    wf = fopen(argv[1], "w");
    if (wf == 0)
    {
        printf("%s file open failed\n", argv[1]);
        return -1;
    }

    while (1)
    {
        printf("INPUT DATA(QUIT-end) : ");
        fgets(buf, sizeof(buf), stdin);

        if (!strncmp(buf, "end", 3))
        {
            break;
        }

        sprintf(buf, "%s\n", buf);
        fputs(buf, wf);
    }

    fclose(wf);
    return 0;
}