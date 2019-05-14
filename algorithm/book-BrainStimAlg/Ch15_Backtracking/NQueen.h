#ifndef NQUEEN_H
#define NQUEEN_H

void PrintSolution(int Columns[], int NumberOfQueens);
int  IsThreatened(int Columns[], int NewRow);
void FindSolutionForQueen(int Columns[], int Row, int NumberOfQueens, int* SolutionCount);

#endif