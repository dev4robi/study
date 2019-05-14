#include <stdio.h>
#include "rbtime.h"

typedef unsigned long long ULONG;

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

ULONG Fibonacci(int N)
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

int main()
{
    int N = 46;

    // N이 19이하일 때는 SlowFibonacci가 더 빠름.
    // N이 20일때는 성능이 비슷함.
    // N이 21일때는 항상 FastFibonacci가 빠름.
    // N이 22이상인 경우부터는 FastFibonacci가 압도적으로 빠름.

    /*
        1) Mat^n = Mat^(n-1) * Mat
        2) Mat^n = Mat^(n/2) * Mat^(n/2)
        
        위 두가지 규칙을 이용하여 빠른 속도로 피보나치 행렬을 구함.
        
        - SlowFibonacci는 재귀를 통한 Double and Conquer로 문제를 접근. ∂(2^n)의 효율이 나오지만,
        FastFibonocci는 재귀를 통한 Divide and Conquer로 문제를 접근. ∂(Log2_n)의 효율이 나옴.
    */

    StartTimer();
    fprintf(stdout, "\n1. FastFibonacci(%d) = %llu\n", N, Fibonacci(N));
    StopTimer();

    StartTimer();
    fprintf(stdout, "\n2. SlowFibonacci(%d) = %llu\n", N, SlowFibonacci(N));
    StopTimer();

    return 0;
}