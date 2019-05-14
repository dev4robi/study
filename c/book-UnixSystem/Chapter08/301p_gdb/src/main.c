#include "common.h"

int main(void)
{
    int result;
    char number[] = "shin";

    printf("===< MAIN >===\n");
    subA();
    subB();

    result = atoi(number);
    printf("RESULT : %d\n", result);

    return 0;
}