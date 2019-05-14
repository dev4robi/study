// [ 이진 탐색 ]
//
// <알고리즘>
// 1. 데이터 집합의 중앙에 있는 요소를 고름
// 2. 중앙 요소의 값과 찾고자 하는 목표 값을 비교
// 3. 목표 값이 중앙 요소보다 작으면 중앙을 기준으로 데이터 집합의 왼편에 대해 새로 검색 수행, 크면 오른편으로 수행
// 4. 찾고자 하는 값을 찾을 때 까지 1~3 반복
//
// <특징>
// 1. 정렬된 자료구조에만 사용할 수 있는 알고리즘
// 2. 탐색속도는 O(log_2 n)
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

//[타이머]/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef long long INT64;

static struct timeval gST_TIMEVAL;

void ResetTimer()
{
    memset(&gST_TIMEVAL, 0x00, sizeof(gST_TIMEVAL));
}

int StartTimer()
{
    return gettimeofday(&gST_TIMEVAL, NULL);
}

int StopTimer()
{
    struct timeval stStopTv;
    INT64  i64Time = 0;
    int    sec, ms, us;

    gettimeofday(&stStopTv, NULL);

    stStopTv.tv_sec  -= gST_TIMEVAL.tv_sec;
    stStopTv.tv_usec -= gST_TIMEVAL.tv_usec;
    i64Time = stStopTv.tv_sec * 1000000 + stStopTv.tv_usec;

    sec = i64Time / 1000000;        // Second
    ms  = i64Time % 1000000 / 1000; // Milli second (1/1000)
    us  = i64Time % 1000;           // Micro second (1/1000000)

    memcpy(&gST_TIMEVAL, &stStopTv, sizeof(gST_TIMEVAL));
    
    fprintf(stdout, "Time Elapsed : %d.%03d%03ds\n", sec, ms, us);

    return i64Time;
}

//[타이머]/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int BinarySearch_Recursive(int Array[], int Left, int Right, int Target) // 재귀로 구현
{
    int MidIdx = (Right + Left) / 2;
    
    if (Left > Right)
    {
        return -1;
    }

    if (Target < Array[MidIdx])
    {
        return BinarySearch_Recursive(Array, Left, MidIdx - 1, Target);
    }
    else if (Target > Array[MidIdx])
    {
        return BinarySearch_Recursive(Array, MidIdx + 1, Right, Target);
    }
    else // Target == Array[MidIdx]
    {
        return MidIdx;
    }
}

int BinarySearch_Loop(int Array[], int Length, int Target) // 반복문으로 구현
{
    int Left, Right, Mid;

    Left = 0;
    Right = Length - 1;

    while (Left <= Right)
    {
        Mid = (Left + Right) / 2;

        if (Target == Array[Mid])
        {
            return Mid;
        }
        else if (Target > Array[Mid])
        {
            Left = Mid + 1;
        }
        else
        {
            Right = Mid - 1;
        }
    }

    return -1;
}

static int PrintArray(int Array[], int Length)
{
    int i;

    for (i = 0; i < Length; ++i)
    {
        fprintf(stdout, "[%d]", Array[i]);
    }

    fprintf(stdout, "\n");
}

static int CompareInt(const void *_elem1, const void *_elem2)
{
    int elem1 = *(int*)_elem1;
    int elem2 = *(int*)_elem2;

    if (elem1 > elem2)
    {
        return 1;
    }
    else if (elem1 < elem2)
    {
        return -1;
    }   
    else
    { 
        return 0;
    }
}

static int BS_Test_main()
{
    int Array[1024];
    int AryLen = sizeof(Array) / sizeof(Array[0]);
    int i;

    for (i = 0; i < AryLen; ++i)
    {
        Array[i] = i;
    }

    fprintf(stdout, "[Array] : ");
    PrintArray(Array, AryLen);

    fprintf(stdout, "\n\n");

    // 재귀로 구현
    StartTimer();
    for (i = -1; i <= AryLen; ++i)
    {
        int idx = BinarySearch_Recursive(Array, 0, AryLen, i);
        fprintf(stdout, "Find_Req(%4d) : %d\n", i, idx);    
    }
    StopTimer();

    fprintf(stdout, "\n\n");

    // 반복문으로 구현
    StartTimer();
    for (i = -1; i <= AryLen; ++i)
    {
        int idx = BinarySearch_Loop(Array, AryLen, i);
        fprintf(stdout, "Find_Lop(%4d) : %d\n", i, idx);    
    }
    StopTimer();

    fprintf(stdout, "\n\n");

    // C언어 표준 라이브러리 사용
    StartTimer();
    for (i = -1; i <= AryLen; ++i)
    {
        int *idx = (int*)bsearch((const void*)&i, (const void*)Array, AryLen, sizeof(Array[0]), CompareInt);
        fprintf(stdout, "Find(%4d)_Std : %d\n", i, (idx != NULL ? *idx : -1));
    }
    StopTimer();

    return 0;
}

int main()
{
    return BS_Test_main();
}