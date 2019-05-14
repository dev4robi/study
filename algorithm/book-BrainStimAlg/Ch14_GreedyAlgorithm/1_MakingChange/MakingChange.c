#include "MakingChange.h"
#include <stdio.h>
#include <stdlib.h>

void GetChange(int Price, int Pay, int CoinUnits[], int Change[], int Size)
{
    int i = 0;
    int ChangeAmount = Pay - Price;

    for (i = 0; i < Size; ++i)
    {
        Change[i] = CountCoins(ChangeAmount, CoinUnits[i]);
        ChangeAmount = ChangeAmount - (CoinUnits[i] * Change[i]);
    }
}

int CountCoins(int Amount, int CoinUnit)
{
    int CoinCount = 0;
    int CurrentAmount = Amount;

    while (CurrentAmount >= CoinUnit)
    {
        ++CoinCount;
        CurrentAmount = CurrentAmount - CoinUnit;
    }

    return CoinCount;
}

void PrintChange(int CoinUnits[], int Change[], int Size)
{
    int i = 0;

    for (i = 0; i < Size; ++i)
    {
        fprintf(stdout, "%8d원 : %d개\n", CoinUnits[i], Change[i]);
    }
}

int Compare(const void* a, const void* b)
{
    int _a = *(int*)a;
    int _b = *(int*)b;

    if (_a < _b) return 1;
    else return -1;
}

int main(void)
{
    int i = 0;
    int Pay = 0;
    int Price = 0;
    int UnitCount = 0;
    int* CoinUnits = NULL;
    int* Change = NULL;

    fprintf(stdout, "동전의 가짓수를 입력 : ");
    scanf("%d", &UnitCount);

    CoinUnits = (int*)malloc(sizeof(int) * UnitCount);
    Change    = (int*)malloc(sizeof(int) * UnitCount);

    for (i = 0; i < UnitCount; ++i)
    {
        fprintf(stdout, "[%d] 번째 동전의 단위 입력 : ", i + 1);
        scanf("%d", &CoinUnits[i]);
    }

    qsort(CoinUnits, UnitCount, sizeof(int), Compare);

    fprintf(stdout, "물건 가격 입력 : ");
    scanf("%d", &Price);

    fprintf(stdout, "손님이 지불한 금액 입력 : ");
    scanf("%d", &Pay);

    GetChange(Price, Pay, CoinUnits, Change, UnitCount);

    PrintChange(CoinUnits, Change, UnitCount);

    return 0;
}