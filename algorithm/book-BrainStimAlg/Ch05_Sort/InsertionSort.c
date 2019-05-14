#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PrintArray(int DataSet[], int Length)
{
    int i;

    for (i = 0; i < Length; ++i)
    {
        fprintf(stdout, "%d ", DataSet[i]);
    }

    fprintf(stdout, "\n");
}

void InsertionSort_Swap(int DataSet[], int Length) // My Code using 'Swap'
{
    int i, j;
    int newLen = Length - 1;

    for (i = 0; i < newLen; ++i)
    {
        for (j = i + 1; j > 0; --j)
        {
            int *left = &DataSet[j - 1];
            int *right = &DataSet[j];

            if (*left > *right)
            {
                int temp = *left;
                *left = *right;
                *right = temp;
            }
        }
    }
}

void InsertionSort_Memmove(int DataSet[], int Length) // My Code using 'memmove()'
{
    int sortedIdx, pivotIdx;
    int newLen = Length - 1;

    for (sortedIdx = 0; sortedIdx < newLen; ++sortedIdx)
    {
        int unsortedIdx = sortedIdx + 1;
        int selected = DataSet[unsortedIdx];
        int pivotMvCnt = 0;

        for (pivotIdx = unsortedIdx; pivotIdx > 0; --pivotIdx)
        {
            int left = DataSet[pivotIdx - 1];
            
            if (left > selected)
            {
                ++pivotMvCnt;
                continue;
            }

            break;
        }

        if (pivotMvCnt > 0)
        {
            memmove(&DataSet[pivotIdx + 1], &DataSet[pivotIdx], pivotMvCnt * sizeof(int));
            DataSet[pivotIdx] = selected;
        }
    }
}

void InsertionSort_Book(int DataSet[], int Length) // Book Code using 'memmove()'
{
    int i = 0;
    int j = 0;
    int value = 0;

    for (i = 1; i < Length; ++i)
    {
        if (DataSet[i - 1] <= DataSet[i]) continue;
        
        value = DataSet[i];

        for (j = 0; j < i; ++j)
        {
            if (DataSet[j] > value)
            {
                memmove(&DataSet[j + 1], &DataSet[j], sizeof(DataSet[0]) * (i - j));
                DataSet[j] = value;
                break;
            }
        }
    }
}

int main()
{
    int DataSetA[] = { 2, 3, 5, 4, 6, 1 };
    int DataSetB[] = { 1, 2, 3, 4, 5, 6 };
    int DataSetC[] = { 6, 5, 4, 3, 2, 1 };
    int DataSetD[] = { 1, 2, 3, 4, 6, 5 };
    int Length = sizeof(DataSetA) / sizeof(DataSetA[0]);

    InsertionSort_Memmove(DataSetA, Length);
    PrintArray(DataSetA, Length);
    fprintf(stdout, "\n\n");

    InsertionSort_Memmove(DataSetB, Length);
    PrintArray(DataSetB, Length);
    fprintf(stdout, "\n\n");

    InsertionSort_Memmove(DataSetC, Length);
    PrintArray(DataSetC, Length);
    fprintf(stdout, "\n\n");

    InsertionSort_Memmove(DataSetD, Length);
    PrintArray(DataSetD, Length);
    fprintf(stdout, "\n\n");

    return 0;
}