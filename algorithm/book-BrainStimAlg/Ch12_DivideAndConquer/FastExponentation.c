#include <stdio.h>
#include <math.h>
#include "rbtime.h"

typedef unsigned long long ULONG;

ULONG SlowPower(int Base, int Exponent)
{
    ULONG rtVal = 1;

    if (Base == 0)
        return 1;

    for (int i = 0; i < Exponent; ++i)
        rtVal *= Base;

    return rtVal;
}

ULONG Power(int Base, int Exponent)
{
    if (Exponent == 1)
        return Base;
    else if (Base == 0)
        return 1;
    
    if (Exponent % 2 == 0)
    {
        ULONG NewBase = Power(Base, Exponent / 2);
        return NewBase * NewBase;
    }
    else
    {
        ULONG NewBase = Power(Base, (Exponent - 1) / 2);
        return (NewBase * NewBase) * Base;
    }
}

int main()
{
    int Base = 2;
    int Exp = 32;
    
    StartTimer();
    fprintf(stdout, "\n1. Slow Pow(%d,%d) = %llu\n", Base, Exp, SlowPower(Base, Exp));
    StopTimer();

    StartTimer();
    fprintf(stdout, "\n2. Fast Pow(%d,%d) = %llu\n", Base, Exp, Power(Base, Exp));
    StopTimer();

    StartTimer();
    fprintf(stdout, "\n3. Std Pow(%d,%d) = %llu\n", Base, Exp, (ULONG)pow(Base, Exp));
    StopTimer();

    return 0;
}