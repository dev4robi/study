#include <stdio.h>
#include <string.h>
#include "BruteForce.h"
#include "KarpRabin.h"
#include "KnuthMorrisPratt.h"
#include "BoyerMoore.h"
#include "rbtime.h"

#define MAX_BUFFER 512

int Strstr(char* Text, int TextSize, int Start, char* Pattern, int PatternSize)
{
    char* ResultPt = NULL;

    if ((ResultPt = strstr(&Text[Start], Pattern)) != NULL) {
        return ((ResultPt - Text) / sizeof(char*));
    }

    return -1;
}

int Test_StringSearch(int argc, char **argv, int (*searchAlgoFp)(char*, int , int , char* , int ))
{
    char* FilePath;
    FILE* fp;

    char Text[MAX_BUFFER];
    char* Pattern;
    int PatternSize = 0;
    int Line = 0;
    int Counter = 0;

    if (argc < 3)
    {
        fprintf(stdout, "Usage: %s <FilePath> <Pattern>\n", argv[0]);
        return -1;
    }

    FilePath = argv[1];
    Pattern = argv[2];

    PatternSize = strlen(Pattern);

    if ((fp = fopen(FilePath, "r")) == NULL)
    {
        fprintf(stdout, "Cannot open file: %s\n", FilePath);
        return -1;
    }

    while (fgets(Text, MAX_BUFFER, fp) != NULL)
    {
        int Position = searchAlgoFp(Text, strlen(Text), 0, Pattern, PatternSize);

        ++Line;

        if (Position >= 0)
        {
            fprintf(stdout, "Line:%d, Column:%d : %s", Line, Position, Text);
            ++Counter;
        }
    }

    fclose(fp);

    return Counter;
}

int main(int argc, char **argv)
{   
    int results = -1;

    // 브루트 포스
    fprintf(stdout, "[ BruteForce Begin ]\n");
    StartTimer();
    results = Test_StringSearch(argc, argv, BruteForce);
    StopTimer();
    fprintf(stdout, ">> Results : %d\n", results);
    fprintf(stdout, "[ BruteForce End ]\n\n");

    // 카프-라빈 알고리즘
    fprintf(stdout, "[ Karp-Rabin Begin ]\n");
    StartTimer();
    results = Test_StringSearch(argc, argv, KarpRabin);
    StopTimer();
    fprintf(stdout, ">> Results : %d\n", results);
    fprintf(stdout, "[ Karp-Rabin End ]\n\n");

    // KMP 알고리즘
    fprintf(stdout, "[ KMP Begin ]\n");
    StartTimer();
    results = Test_StringSearch(argc, argv, KnuthMorrisPratt);
    StopTimer();
    fprintf(stdout, ">> Results : %d\n", results);
    fprintf(stdout, "[ KMP End ]\n\n");

    // 보이어-무어 알고리즘
    fprintf(stdout, "[ Boyer-Moore Begin ]\n");
    StartTimer();
    results = Test_StringSearch(argc, argv, BoyerMoore);
    StopTimer();
    fprintf(stdout, ">> Results : %d\n", results);
    fprintf(stdout, "[ Boyer-Moore End ]\n\n");

    // 표준 라이브러리 strstr()
    fprintf(stdout, "[ Strstr Begin ]\n");
    StartTimer();
    results = Test_StringSearch(argc, argv, Strstr);
    StopTimer();
    fprintf(stdout, ">> Results : %d\n", results);
    fprintf(stdout, "[ Strstr End ]\n\n");

    return 0;
}