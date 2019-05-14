#include "KarpRabin.h"

static int powi(int n, int m)
{
    int result = 1;
    int i;

    for (i = 0; i < m; ++i)
    {
        result *= n;
    }

    return result;
}

int KarpRabin(char* Text, int TextSize, int Start, char* Pattern, int PatternSize)
{
    int i = 0;
    int j = 0;                                                      // m = PatternSize
    int Coefficient = powi(2, PatternSize - 1);                     // C = 2^(m-1)
    int HashText = RabinsFingerprintHash(Text, PatternSize);        // (Text[i] * (2^m) + Text[i + 1] * (2^(m-1)) + ... + Text[i + m] * (2^0)) mod q
    int HashPattern = RabinsFingerprintHash(Pattern, PatternSize);

    for (i = Start; i <= TextSize - PatternSize; ++i)
    {
        HashText = ReHash(Text, i, PatternSize, HashText, Coefficient); // ((2 * (HashText - Text[i] * 2^4)) + Text[i + m + 1]) mod q
                                                                        // 이전 해싱값으로 다음 해싱값을 O(1) 상수 시간에 구할 수 있어서 가능한 탐색기법.
        if (HashPattern == HashText)
        {
            // 해시값이 일치하면 Brute force으로 1:1 비교
            for (j = 0; j < PatternSize; ++j)
            {
                if (Text[i + j] != Pattern[j])
                    break;
            }

            if (j >= PatternSize)
                return i;
        }
    }

    return -1;
}

static int RabinsFingerprintHash(char* String, int Size)
{
    int i = 0;
    int HashValue = 0;

    for (i = 0; i < Size; ++i)
    {
        HashValue += (String[i] * powi(2, Size - (1 + i)));
    }

    return HashValue;
}

static int ReHash(char* String, int Start, int Size, int HashPrev, int Coefficient)
{
    if (Start == 0)
        return HashPrev;

    return String[Start + Size - 1] + ((HashPrev - Coefficient * String[Start - 1]) * 2);
}