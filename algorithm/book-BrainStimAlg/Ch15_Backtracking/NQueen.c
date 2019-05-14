#include "NQueen.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 체스판 출력
void PrintSolution(int Columns[], int NumberOfQueens)
{
    int i = 0, j = 0;

    for (i = 0; i < NumberOfQueens; ++i)
    {
        for (j = 0; j < NumberOfQueens; ++j)
        {
            if (Columns[i] == j)
                fprintf(stdout, "Q");
            else
                fprintf(stdout, ".");
        }

        fprintf(stdout, "\n");
    }
}

// 퀸끼리 위험범위인지 체크
int IsThreatened(int Columns[], int NewRow)
{
    int CurrentRow = 0;
    int Threatened = 0;

    while (CurrentRow < NewRow)
    {
        if (Columns[NewRow] == Columns[CurrentRow] ||
            abs(Columns[NewRow] - Columns[CurrentRow]) == abs(NewRow - CurrentRow))
        {
            Threatened = 1;
            break;
        }

        ++CurrentRow;
    }

    return Threatened;
}

// N-Queen 해결 (재귀)
void FindSolutionForQueen(int Columns[], int Row, int NumberOfQueens, int* SolutionCount)
{
    static int cnt = 0;

    //PrintSolution(Columns, NumberOfQueens);
    //fprintf(stdout, "Cnt:%d, Row:%d, NoQ:%d, SC:%d\n", cnt++, Row, NumberOfQueens, *SolutionCount);
    //getchar();

    if (IsThreatened(Columns, Row))
        return;

    if (Row == NumberOfQueens - 1)
    {
        fprintf(stdout, "Solution #%d : \n", ++(*SolutionCount));
        PrintSolution(Columns, NumberOfQueens);
    }
    else
    {
        int i = 0;

        for (i = 0; i < NumberOfQueens; ++i)
        {
            Columns[Row + 1] = i;
            FindSolutionForQueen(Columns, Row + 1, NumberOfQueens, SolutionCount);
        }
    }
}

// 메인
int main(int argc, char** argv)
{
    int i = 0;
    int numberOfQueens = 0;
    int solutionCount = 0;
    int* columns = NULL;

    if (argc < 2)
    {
        fprintf(stdout, "Usage: %s <Number Of Queens>", argv[0]);
        return 1;
    }

    numberOfQueens = atoi(argv[1]);
    columns = (int*)calloc(numberOfQueens, sizeof(int));

    for (i = 0; i < numberOfQueens; ++i)
    {
        columns[0] = i;
        FindSolutionForQueen(columns, 0, numberOfQueens, &solutionCount);
    }

    free(columns);
    return 0;
}