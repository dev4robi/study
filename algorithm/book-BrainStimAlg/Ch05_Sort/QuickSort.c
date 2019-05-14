#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void Swap(int* A, int* B)
{
    int Temp = *A;

    *A = *B;
    *B = Temp;    
}

void PrintArray(int DataSet[], int Length)
{
    int i;

    for (i = 0; i < Length; ++i)
    {
        fprintf(stdout, "%d ", DataSet[i]);
    }

    fprintf(stdout, "\n");
}

void PrintArraySmart(int DataSet[], int Length, int LeftIdx, int PivotIdx, int RightIdx) {
    char Buffer[Length * 10];
    int bufIdx = 0;
    int i;

    char *lrSym = "[%d] ";
    char *pivSym = "(%d) ";
    char *btSym = "<%d> ";
    char *nSym = " %d  ";
    char *pSym;

    memset(Buffer, 0x00, sizeof(Buffer));

    for (i = 0; i < Length; ++i) {
        if (i == LeftIdx && i == RightIdx) {
            pSym = btSym;
        }
        else if (i == LeftIdx || i == RightIdx) {
            pSym = lrSym;
        }
        else if (i == PivotIdx) {
            pSym = pivSym;
        }
        else {
            pSym = nSym;
        }

        {
            char buf[20];
            int bufLen;

            memset(buf, 0x00, sizeof(buf));
            sprintf(buf, pSym, DataSet[i]);
            bufLen = strlen(buf);
            memcpy(&Buffer[bufIdx], buf, bufLen);
            bufIdx += bufLen;
        }
    }

    fprintf(stdout, "%s\n", Buffer);
}

int Partition(int DataSet[], int Left, int Right)
{
    int First = Left;
    int Pivot = DataSet[First];

    ++Left;

    while (Left <= Right)
    {
        while (DataSet[Left] <= Pivot && Left < Right) ++Left;
        
        while (DataSet[Right] > Pivot && Left <= Right) --Right;

        if (Left < Right) {
            Swap(&DataSet[Left], &DataSet[Right]);
        }
        else {
            break;
        }
    }

    Swap(&DataSet[First], &DataSet[Right]);

    return Right;
}

void QuickSort(int DataSet[], int Left, int Right)
{
    if (Left < Right)
    {
        int Index = Partition(DataSet, Left, Right);

        QuickSort(DataSet, Left, Index - 1);
        QuickSort(DataSet, Index + 1, Right);
    }
}

int COUNT = 0;

void QuickSort_Middle(int DataSet[], int Left, int Right)
{
    int nMidIdx = (Left + Right) / 2;
    int nLeftIdx = Left;
    int nRightIdx = Right;
    int nPivot = DataSet[nMidIdx];

    do {
        PrintArraySmart(DataSet, Right - Left + 1, nLeftIdx, nMidIdx, nRightIdx);
        while (nLeftIdx < nRightIdx && DataSet[nLeftIdx] < nPivot) { ++COUNT; ++nLeftIdx; PrintArraySmart(DataSet, Right - Left + 1, nLeftIdx, nMidIdx, nRightIdx); }
        while (nLeftIdx < nRightIdx && nPivot < DataSet[nRightIdx]) { ++COUNT; --nRightIdx; PrintArraySmart(DataSet, Right - Left + 1, nLeftIdx, nMidIdx, nRightIdx); }

        fprintf(stdout, "[SWAP] : LeftIdx: %d(%d), Pivot: %d, RightIdx: %d(%d)\n", nLeftIdx, DataSet[nLeftIdx], nPivot, nRightIdx, DataSet[nRightIdx]);
        Swap(&DataSet[nLeftIdx], &DataSet[nRightIdx]);
        PrintArraySmart(DataSet, Right - Left + 1, nLeftIdx, nMidIdx, nRightIdx);

        if (nLeftIdx == nRightIdx) {
            fprintf(stdout, "[SWAP] : LeftIdx: %d(%d), Pivot: %d, RightIdx: %d(%d)\n", nLeftIdx, DataSet[nLeftIdx], nPivot, nRightIdx, DataSet[nRightIdx]);
            Swap(&DataSet[nLeftIdx], &DataSet[nMidIdx]);
            PrintArraySmart(DataSet, Right - Left + 1, nLeftIdx, nMidIdx, nRightIdx);
            break;
        }
        else {
            ++nLeftIdx;
            --nRightIdx;
        }
        
    } while (nLeftIdx < nRightIdx);

    QuickSort(DataSet, Left, nLeftIdx - 1);
    QuickSort(DataSet, nRightIdx + 1, Right);
}

void GenRandomAry(int DataSet[], int Length, int Min, int Max) {
    int i;

    for (i = 0; i < Length; ++i) {
        DataSet[i] = (rand() % (Max - Min)) + Min;
    }
}

int CompareInt(const void *a, const void *b)
{
    return *(int*)a > *(int*)b;
}

int CheckSorted(int DataSet[], int Length) {
    int newLen = Length - 1;
    int i;

    for (i = 0; i < newLen; ++i) {
        if (DataSet[i] > DataSet[i + 1]) {
            fprintf(stdout, "Err[%d] : %d > %d\n", i, DataSet[i], DataSet[i + 1]);
            return 0;
        }
    }

    return 1;
}

int main(void)
{
    int DataSetA[] = { 6, 4, 2, 3, 1, 5 };
    int DataSetB[] = { 1, 2, 3, 4, 5, 6 };
    int DataSetC[] = { 6, 5, 4, 3, 2, 1 };
    int DataSetD[] = { 1, 2, 3, 4, 6, 5 };
    int DataSetE[] = { 6, 4, 2, 3, 1, 5 };
    int DataSetX[10];
    int Length = sizeof(DataSetA) / sizeof(DataSetA[0]);
    int LengthX = sizeof(DataSetX) / sizeof(DataSetX[0]);
    int i;
    int nonSortCnt = 0;

    srand(time(NULL));
    
    for (i = 0; i < 10; ++i) {
    fprintf(stdout, "QuickSort_Middle()\n");
    GenRandomAry(DataSetX, LengthX, 0, LengthX * 5);
    fprintf(stdout, "Before > ");
    PrintArray(DataSetX, LengthX);
    QuickSort_Middle(DataSetX, 0, LengthX - 1);
    fprintf(stdout, "After > ");
    PrintArray(DataSetX, LengthX);
    fprintf(stdout, "COUNT : %d\n", COUNT);
    if (!CheckSorted(DataSetX, LengthX)) {
        ++nonSortCnt;
        fprintf(stdout, "RESULT : Not Sorted!\n");
    }
    fprintf(stdout, "\n");
    }
    fprintf(stdout, "TOTAL_NON_CNT : %d\n", nonSortCnt);
/*
    fprintf(stdout, "QuickSort_Middle()\n");
    fprintf(stdout, "Before > ");
    PrintArray(DataSetA, Length);
    QuickSort_Middle(DataSetA, 0, Length - 1);
    fprintf(stdout, "After > ");
    PrintArray(DataSetA, Length);
    fprintf(stdout, "\n");

    fprintf(stdout, "QuickSort()\n");
    fprintf(stdout, "Before > ");
    PrintArray(DataSetB, Length);
    QuickSort(DataSetB, 0, Length - 1);
    fprintf(stdout, "After > ");
    PrintArray(DataSetB, Length);
    fprintf(stdout, "\n");

    fprintf(stdout, "QuickSort()\n");
    fprintf(stdout, "Before > ");
    PrintArray(DataSetC, Length);
    QuickSort(DataSetC, 0, Length - 1);
    fprintf(stdout, "After > ");
    PrintArray(DataSetC, Length);
    fprintf(stdout, "\n");

    fprintf(stdout, "QuickSort()\n");
    fprintf(stdout, "Before > ");
    PrintArray(DataSetD, Length);
    QuickSort(DataSetD, 0, Length - 1);
    fprintf(stdout, "After > ");
    PrintArray(DataSetD, Length);
    fprintf(stdout, "\n");

    fprintf(stdout, "qsort()\n");
    fprintf(stdout, "Before > ");
    PrintArray(DataSetE, Length);
    qsort(DataSetE, Length, sizeof(int), &CompareInt); // <stdlib.h> c-standard quicksort function
    fprintf(stdout, "After > ");
    PrintArray(DataSetE, Length);
    fprintf(stdout, "\n");
*/
    return 0;
}