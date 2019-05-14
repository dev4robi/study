#include <stdio.h>
#include <stdlib.h>
#include "rbtime.h"

typedef unsigned long long ULONG;

// 여기부터 Ch12 분할 정복 피보나치 구하기 코드
typedef struct tagMatrix2x2
{
    ULONG Data[2][2];

} Matrix2x2;

Matrix2x2 Matrix2x2_Multiply(Matrix2x2 A, Matrix2x2 B)
{
    Matrix2x2 C;

    C.Data[0][0] = A.Data[0][0] * B.Data[0][0] + A.Data[0][1] * B.Data[1][0];
    C.Data[0][1] = A.Data[0][0] * B.Data[1][0] + A.Data[0][1] * B.Data[1][1];
    C.Data[1][0] = A.Data[1][0] * B.Data[0][0] + A.Data[1][1] * B.Data[1][0];
    C.Data[1][1] = A.Data[1][0] * B.Data[1][0] + A.Data[1][1] * B.Data[1][1];

    return C;
}

Matrix2x2 Matrix2x2_Power(Matrix2x2 A, int n)
{
    if (n > 1)
    {
        A = Matrix2x2_Power(A, n / 2);
        A = Matrix2x2_Multiply(A, A);
    
        if (n & 1) // n이 홀수 (n % 2 != 0)
        {
            Matrix2x2 B;
            B.Data[0][0] = 1;       B.Data[0][1] = 1;
            B.Data[1][0] = 1;       B.Data[1][1] = 0;

            A = Matrix2x2_Multiply(A, B);
        }
    }

    return A;
}

ULONG DnqFibonacci(int N)
{
    Matrix2x2 A;
 
    A.Data[0][0] = 1;       A.Data[0][1] = 1;
    A.Data[1][0] = 1;       A.Data[1][1] = 0;

    A = Matrix2x2_Power(A, N);

    return A.Data[0][1];
}

ULONG SlowFibonacci(int N)
{
    if (N == 0)
        return 0;
    else if (N == 1 || N == 2)
        return 1;
    
    return SlowFibonacci(N - 1) + SlowFibonacci(N - 2);
}
// 여기까지 Ch12 분할 정복 피보나치 구하기 코드

// 여기부터 Ch13 동적 계획법 피보나치 구하기 코드
ULONG DpFibonacci(int N)
{
    int i;
    ULONG Result;
    ULONG* FibonacciTable;

    if (N == 0 || N == 1)
        return N;

    FibonacciTable = (ULONG*)malloc(sizeof(ULONG) * (N + 1));

    FibonacciTable[0] = 0;
    FibonacciTable[1] = 1;

    for (i = 2; i <= N; ++i)
    {
        FibonacciTable[i] = FibonacciTable[i - 1] + FibonacciTable[i - 2];
    }

    Result = FibonacciTable[N];

    free (FibonacciTable);

    return Result;
}

int main()
{
    int N = 46;

    // N이 19이하일 때는 SlowFibonacci가 더 빠름.
    // N이 20일때는 성능이 비슷함.
    // N이 21일때는 항상 DnqFibonacci가 빠름.
    // N이 22이상인 경우부터는 DnqFibonacci가 압도적으로 빠름.

    // 이론상 Dnq가 가장 빨라야 하지만, N을 100으로 키워도 Dp가 가장 빠르다.
    // Dnq코드가 최적화가 필요하거나, 함수 call 부하/행렬연산 부하가 확실히 느껴지는듯.
    // N이 8550정도로 커지면 Dnq와 Dp의 시간이 비슷해지고, 15000정도 되면 Dnq가 항상 빨라지지만, 둘 다 0.0001~2초 대로 처리할 수 있다.

    /*        
        - SlowFibonacci는 재귀를 통한 Double and Conquer로 문제를 접근. ∂(2^n)의 효율이 나옴.
        - DnqFibonacci는 재귀를 통한 Divide and Conquer로 문제를 접근. ∂(Log2_n)의 효율이 나옴.
        - DpFibonacci는 동적 계획법(Dynamic Programming)으로 문제를 접근. ∂(n)의 효율이 나옴.
    */

    StartTimer();
    fprintf(stdout, "\n1. Divide and ConqureFibonacci(%d) = %llu\n", N, DnqFibonacci(N));
    StopTimer();

    //StartTimer();
    //fprintf(stdout, "\n2. SlowFibonacci(%d) = %llu\n", N, SlowFibonacci(N)); // 무지 느려서 컴퓨터 멈춘줄 알았다 (N=46 -> 약 9.073s)
    //StopTimer();

    StartTimer();
    fprintf(stdout, "\n3. DpFibonacci(%d) = %llu\n", N, DpFibonacci(N));
    StopTimer();

    return 0;
}