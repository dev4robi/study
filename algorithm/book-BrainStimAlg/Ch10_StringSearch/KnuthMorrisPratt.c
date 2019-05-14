#include "KnuthMorrisPratt.h"

// #define CUSTOM_CODE
#ifdef CUSTOM_CODE
void Preprocess(char* Pattern, int PatternSize, int* Border)
{
    int i, j;

    if (Border == NULL)
        return;

    for (i = 1; i < PatternSize; ++i)
    {
        while (j > 0 && Pattern[i] != Pattern[j])
            j = Border[j - 1];
        
        if (Pattern[i] == Pattern[j])
            Border[i] = ++j;
    }

    for (int z = 0; z < PatternSize; ++z) {
        fprintf(stdout, "%d ", Border[z]);
    }
}

int KnuthMorrisPratt(char* Text, int TextSize, int Start, char* Pattern, int PatternSize)
{
    int Border[PatternSize + 1];
    Preprocess(Pattern, PatternSize, Border);
    return 0;
}

#else
int KnuthMorrisPratt(char* Text, int TextSize, int Start, char* Pattern, int PatternSize)
{
    int i = Start;
    int j = 0;
    int Position = -1;
    int* Border = (int*)calloc(PatternSize + 1, sizeof(int));

    Preprocess(Pattern, PatternSize, Border);

    while (i < TextSize)
    {
        while (j >= 0 && Text[i] != Pattern[j])
            j = Border[j];
        
        ++i;
        ++j;

        if (j == PatternSize)
        {
            Position = i - j;
            break;
        }
    }

    free(Border);

    return Position;
}

void Preprocess(char* Pattern, int PatternSize, int* Border)
{
    int i = 0;
    int j = -1;

    Border[0] = -1;

    while (i < PatternSize)
    {
        while (j > -1 && Pattern[i] != Pattern[j])
            j = Border[j];
        
        ++i;
        ++j;
        Border[i] = j;
    }
}

#endif