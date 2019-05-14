#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>

static const char *CHECKONE = "./checkOne.sh";

void runCheckOne(char *args, char result[8])
{
    char cmd[32];
    FILE *fp;

    sprintf(cmd, "%s %s", CHECKONE, args);

    if ((fp = popen(cmd, "rw")) == NULL)
    {
        fprintf(stderr, "\nrunCheckOne() Failure to open the pile\n");
        strncpy(result, "NULLRES\0", 8);
        return;
    }

    cmd[0] = 'F'; cmd[1] = 'F'; cmd[2] = 'F'; cmd[3] = 'F';
    cmd[4] = 'F'; cmd[5] = 'F'; cmd[6] = 'F'; cmd[7] = '\0';

    fread(cmd, 1, 7, fp);

    if (!strncmp("FFFF", cmd, 4))
    {
        pclose(fp);
        fprintf(stderr, "\nrunCheckOne() fread failed\n");
        strncpy(result, "NULLRES\0", 8);
        pclose(fp);
        return;
    }

    strncpy(result, cmd, 8);
    pclose(fp);
}

// .......