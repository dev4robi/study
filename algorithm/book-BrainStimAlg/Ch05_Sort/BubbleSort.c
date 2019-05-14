#include <stdio.h>

void PrintArray(int DataSet[], int Length)
{
    int i;

    for (i = 0; i < Length; ++i)
    {
        fprintf(stdout, "%d ", DataSet[i]);
    }

    fprintf(stdout, "\n");
}

void BubbleSort(int DataSet[], int Length)
{
    int i, j;
    int newLen = Length - 1;

    for (i = 0; i < newLen; ++i)
    {
        int maxJ = newLen - i;
        int chkSwap = 0;

        for (j = 0; j < maxJ; ++j)
        {
            if (DataSet[j] > DataSet[j + 1])
            {
                int temp = DataSet[j];
                DataSet[j] = DataSet[j + 1];
                DataSet[j + 1] = temp;
                chkSwap = 1;
            }
        }
        
        if (chkSwap == 0) break;
    }
}

int main(void)
{
    int DataSetA[] = { 6, 4, 2, 3, 1, 5 };
    int DataSetB[] = { 1, 2, 3, 4, 5, 6 };
    int DataSetC[] = { 6, 5, 4, 3, 2, 1 };
    int DataSetD[] = { 1, 2, 3, 4, 6, 5 };
    int Length = sizeof(DataSetA) / sizeof(DataSetA[0]);
    int i;

    BubbleSort(DataSetA, Length);
    PrintArray(DataSetA, Length);
    fprintf(stdout, "\n\n");

    BubbleSort(DataSetB, Length);
    PrintArray(DataSetB, Length);
    fprintf(stdout, "\n\n");

    BubbleSort(DataSetC, Length);
    PrintArray(DataSetC, Length);
    fprintf(stdout, "\n\n");

    BubbleSort(DataSetD, Length);
    PrintArray(DataSetD, Length);
    fprintf(stdout, "\n\n");

    return 0;
}