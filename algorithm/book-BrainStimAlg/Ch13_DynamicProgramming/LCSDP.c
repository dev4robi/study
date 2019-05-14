// LCSDP (Longest Common Subsequence with Dynamic Programming : 최장 공통 부분순서 with 동적 계획법)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct structLCSTable
{
    int** Data;
} LCSTable;

int LCS(char* X, char* Y, int i, int j, LCSTable* Table)
{
    int m = 0, n = 0;

    for (m = 0; m <= i; ++m)        // 0행 0으로 초기화
        Table->Data[m][0] = 0;

    for (n = 0; n <= j; ++n)        // 0열 0으로 초기화
        Table->Data[0][n] = 0;

    for (m = 1; m <= i; ++m)
    {
        for (n = 1; n <= j; ++n)
        {
            if (X[m - 1] == Y[n - 1])
                Table->Data[m][n] = Table->Data[m - 1][n - 1] + 1; // 이전 행렬 데이터 + 1
            else
            {
                if (Table->Data[m][n - 1] >= Table->Data[m - 1][n])
                    Table->Data[m][n] = Table->Data[m][n - 1];
                else
                    Table->Data[m][n] = Table->Data[m - 1][n];
            }
        }
    }

    return Table->Data[i][j]; // 가장 우측 하단
}

void LCS_TraceBack(char* X, char* Y, int m, int n, LCSTable* Table, char* LCS)
{
    if (m == 0 || n == 0)
        return;

    if (Table->Data[m][n] > Table->Data[m][n - 1] &&
        Table->Data[m][n] > Table->Data[m - 1][n] &&
        Table->Data[m][n] > Table->Data[m - 1][n - 1])
    {
        char TempLCS[100];
        strcpy(TempLCS, LCS);
        sprintf(LCS, "%c%s", X[m - 1], TempLCS);

        LCS_TraceBack(X, Y, m - 1, n - 1, Table, LCS);
    }
    else if (Table->Data[m][n] > Table->Data[m - 1][n] &&
             Table->Data[m][n] == Table->Data[m][n - 1])
    {
        LCS_TraceBack(X, Y, m, n - 1, Table, LCS);
    }
    else
    {
        LCS_TraceBack(X, Y, m - 1, n, Table, LCS);
    }
}

void LCS_PrintTable(LCSTable* Table, char* X, char* Y, int LEN_X, int LEN_Y)
{
    int i = 0, j = 0;

    fprintf(stdout, "%4s", "");

    for (i = 0; i < LEN_Y; ++i)
        fprintf(stdout, "%c ", Y[i]);
    
    fprintf(stdout, "\n");

    for (i = 0; i < LEN_X + 1; ++i)
    {
        if (i == 0)
            fprintf(stdout, "%2s", "");
        else
            fprintf(stdout, "%-2c", X[i - 1]);

        for (j = 0; j < LEN_Y + 1; ++j)
            fprintf(stdout, "%d ", Table->Data[i][j]);

        fprintf(stdout, "\n");
    }
}

int main()
{
    char* X = "GOOD MORNING.";
    char* Y = "GUTEN MORGEN.";
    char* Result;

    int LEN_X = strlen(X);
    int LEN_Y = strlen(Y);

    int i = 0;
    int j = 0;
    int Length = 0;

    LCSTable Table;

    Table.Data = (int**)malloc(sizeof(int*) * (LEN_X + 1));

    for (i = 0; i < LEN_X + 1; ++i)
    {
        Table.Data[i] = (int*)malloc(sizeof(int) * (LEN_Y + 1));
        memset(Table.Data[i], 0, sizeof(int) * (LEN_Y + 1));
    }

    Length = LCS(X, Y, LEN_X, LEN_Y, &Table);

    LCS_PrintTable(&Table, X, Y, LEN_X, LEN_Y);

    Result = (char*)malloc(sizeof(Table.Data[LEN_X][LEN_Y] + 1));
    sprintf(Result, "%c", '\0');

    LCS_TraceBack(X, Y, LEN_X, LEN_Y, &Table, Result);

    fprintf(stdout, "\n");
    fprintf(stdout, "LCS:\"%s\" (Length:%d)\n", Result, Length);

    return 0;
}