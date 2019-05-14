#include "SequentialSearch.h"

// [순차 탐색 - 전진 이동법(연결리스트)]
// 찾고자 하는 데이터를 찾으면 배열/리스트의 맨 앞쪽으로 이동시킨다.
// 그 효과로 동일 데이터를 순차 탐색시에는 조금 더 빠르게 탐색이 가능해진다.
Node* SSL_MoveToFront(Node** Head, int Target)
{
    Node* Current = (*Head);
    Node* Prev = NULL;
    Node* Match = NULL;

    while (Current != NULL)
    {
        if (Current->Data == Target)
        {
            Match = Current;

            if (Prev != NULL)
            {
                Prev->pNextNode = Current->pNextNode;
                Current->pNextNode = (*Head);
                (*Head) = Current;
            }

            break;
        }
        else
        {
            Prev = Current;
            Current = Current->pNextNode;
        }
    }

    return Match;
}

// [순차 탐색 - 전진 이동법(배열)]
int Ary_MoveToFront(int Array[], int Length, int Target)
{
    int i, j;

    for (i = 0; i < Length; ++i)
    {
        if (Array[i] == Target)
        {
            for (j = i; j > -1; --j)
            {
                Array[j] = Array[j - 1];
            }

            Array[0] = Target;
            return 0;
        }
    }

    return -1;
}

// [순차 탐색 - 전위법(연결리스트)]
// 찾고자 하는 데이터를 찾으면 바로 앞 데이터와 교환한다.
// 자주 찾게 될수록 데이터가 점점 앞으로 이동하여 점차 검색이 빨라지게 된다.
Node* SLL_Transpose(Node** Head, int Target)
{
    Node* Current = (*Head);
    Node* PrePrev = NULL;
    Node* Prev = NULL;
    Node* Match = NULL;

    while (Current != NULL)
    {
        Match = Current;

        if (Current->Data == Target)
        {
            if (Prev != NULL)
            {
                if (PrePrev != NULL)
                    PrePrev->pNextNode = Current;
                else
                    (*Head) = Current;
                
                Prev->pNextNode = Current->pNextNode;
                Current->pNextNode = Prev;
            }

            break;
        }
        else
        {
            if (Prev != NULL)
                PrePrev = Prev;
            
            Prev = Current;
            Current = Current->pNextNode;
        }
    }

    return Match;
}


// [순차 탐색 - 전위법(배열)]
int Ary_Transpose(int Array[], int Length, int Target)
{
    int i;

    for (i = 0; i < Length; ++i)
    {
        if (Array[i] == Target)
        {
            if (i > 0)
            {
                int Temp = Array[i - 1];

                Array[i - 1] = Array[i];
                Array[i] = Temp;
            }

            return i;
        }
    }

    return -1;
}

// [순차 탐색 - 계수법(이론만)]
// 찾고자 하는 데이터를 찾으면 별도의 공간에 저장해둠.
// ...

static void PrintArray(int Array[], int Length)
{
    int i;

    for (i = 0; i < Length; ++i)
    {
        fprintf(stdout, "[%d]", Array[i]);
    }

    fprintf(stdout, "\n");
}

static int SSL_Test_main()
{
    int Array[] = { 1, 4, 2, 3, 7, 6, 8, 9, 0 };
    int AryLen = sizeof(Array) / sizeof(Array[0]);
    int i;

    // 순차 탐색 - 전진이동법(배열)
    fprintf(stdout, "[Array] : ");
    PrintArray(Array, AryLen);

    fprintf(stdout, "Find(7) : ");
    Ary_MoveToFront(Array, AryLen, 7);
    PrintArray(Array, AryLen);

    fprintf(stdout, "Find(9) : ");
    Ary_MoveToFront(Array, AryLen, 9);
    PrintArray(Array, AryLen);

    fprintf(stdout, "Find(0) : ");
    Ary_MoveToFront(Array, AryLen, 0);
    PrintArray(Array, AryLen);

    fprintf(stdout, "\n\n");

    // 순차 탐색 - 전위법(배열)
    fprintf(stdout, "[Array] : ");
    PrintArray(Array, AryLen);

    for (i = 0; i < 3; ++i)
    {
        fprintf(stdout, "Find(7) : ");
        Ary_Transpose(Array, AryLen, 7);
        PrintArray(Array, AryLen);
    }

    fprintf(stdout, "Find(8) : ");
    Ary_Transpose(Array, AryLen, 8);
    PrintArray(Array, AryLen);

    return 0;
}

int main()
{
    return SSL_Test_main();
}