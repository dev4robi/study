#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
    char buf[2048];
    char cmd[256];
    FILE *pipe;

    sprintf(cmd, "ls -l");
    printf("cmd:%s\n", cmd);

    if ((pipe = popen(cmd, "r")) == NULL)
    {
        printf("popen() Error! (%s)\n", strerror(errno));
        return -1;
    }

    fread(buf, sizeof(buf), sizeof(buf), pipe);
    printf("buf:%s\n", buf);
    pclose(pipe);
    return 0;
}