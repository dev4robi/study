#include "Calculator.h"

char NUMBER[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' };

// 숫자('0~9', '.')이면 1반환 그 외 0반환
int IsNumber(char Cipher)
{
    int i = 0;
    int ArrayLength = sizeof(NUMBER);

    for (i = 0; i < ArrayLength; ++i)
    {
        if (Cipher == NUMBER[i])
        {
            return 1;
        }
    }

    return 0;
}

// 다음 토큰과 타입을 구함
unsigned int GetNextToken(char *Expression, char *Token, int *TYPE)
{
    unsigned int i = 0;

    for (i = 0; Expression[i] != 0; ++i)
    {
        Token[i] = Expression[i];

        if (IsNumber(Expression[i]) == 1)
        {
            *TYPE = OPERAND;

            if (IsNumber(Expression[i + 1]) != 1)
            {
                break;
            }
        }
        else
        {
            *TYPE = Expression[i];
            break;
        }
    }

    Token[++i] = '\0';
    return i;
}

// 연산자의 우선순위 구함
int GetPriority(char Operator, int InStack)
{
    int Priority = -1;

    switch (Operator)
    {
        case LEFT_PARENTHESIS:
            Priority = InStack ? 3 : 0;
            break;
        case MULTIPLY:
        case DIVIDE:
            Priority = 1;
            break;
        case PLUS:
        case MINUS:
            Priority = 2;
            break;
    }

    return Priority;
}

// 스택안 연산자 우선순위가 낮으면 1반환 그 외 0반환
int IsPrior(char OperatorInStack, char OperatorInToken)
{
    return (GetPriority(OperatorInStack, 1) > GetPriority(OperatorInToken, 0));
}

// 연산 Infix방식의 연산식을 Postfix방식으로 변경 ( "2*1+5*3" => "2 1 *5 3 *+" )
void GetPostfix(char *InfixExpression, char *PostfixExpression)
{
    // [ Postfix 변환 규칙 요약 - Dijkstra Postfix Algorithm ]
    // 1. 입력받은 중위 표기식에서 토큰을 읽음
    // 2. 토큰이 피연산자이면 토큰을 결과에 출력
    // 3. 토큰이 연산자(괄호 포함)인 경우, 스택의 최상의 노드에 담겨있는 연산자가 토큰보다 우선순위가 높은지 검사.
    //    검사 결과가 참(1)이면 최상위 노드를 스택에서 꺼내 결과에 출력. 이 검사 작업을 반복해서 수행하고 그 결과가 거짓(0)
    //    이거나 스택이 비게 되면 작업을 중단. 검사 작업이 끝난 후에는 토큰을 스택에 삽입.
    //    (스택의 최상위 노드보다 우선순위가 높은 연산자는 존재하지 않게 됨.)
    // 4. 토큰이 오른쪽 괄호이면 최상위 노드에 왼쪽 괄호가 올 때까지 스택에 제거 연산을 수행하고 제거한 노드에 담긴 연산자를 출력.
    //    왼쪽 괄호를 만나면 제거만 하고 출력하지 않음.
    // 5. 중위 표기식에 더 읽을 것이 없다면 빠져나가고, 더 읽을 것이 있다면 1부터 다시 반복.
    // 6. 스택의 남은 연산자들을 중위 표기식에 기록.

    LinkedListStack *Stack;
    char Token[32];
    int Type = -1;
    unsigned int Position = 0;
    unsigned int Length = strlen(InfixExpression);

    LLS_CreateStack(&Stack);

    // 5
    while (Position < Length) // PostfixExpression에 숫자를 이어붙이고, Stack에 연산자 푸시
    {
        // 1
        Position += GetNextToken(&InfixExpression[Position], Token, &Type);

        // 2
        if (Type == OPERAND) // 토큰이 숫자면 " "로 구분하여 이어붙임
        {
            strcat(PostfixExpression, Token);
            strcat(PostfixExpression, " ");
        }
        // 4
        else if (Type == RIGHT_PARENTHESIS) // 토큰이 ')'
        {
            while (!LLS_IsEmpty(Stack)) // '('를 만나거나 스택이 텅비기 전 까지 스택 Pop()하고 그 결과를 이어붙임
            {
                Node *Popped = LLS_Pop(Stack);

                if (Popped->Data[0] == LEFT_PARENTHESIS) // 토큰이 '('
                {
                    LLS_DestroyNode(Popped);
                    break;
                }
                else // 토큰이 연산자
                {
                    strcat(PostfixExpression, Popped->Data);
                    strcat(PostfixExpression, " ");
                    LLS_DestroyNode(Popped);
                }
            }
        }
        // 3
        else // 토큰이 연산자 ( '(', '+', '-', '*', '/' )
        {
            while (!LLS_IsEmpty(Stack) && !IsPrior(LLS_Top(Stack)->Data[0], Token[0])) // 스택이 비거나 스택의 Top보다 토큰의 연산자가 우선순위가 낮을때까지 Pop하면서 그 결과를 이어붙임
            {
                Node *Popped = LLS_Pop(Stack);

                if (Popped->Data[0] != LEFT_PARENTHESIS) // 토큰이 '('가 아니면 이어붙임
                {
                    strcat(PostfixExpression, Popped->Data);
                    strcat(PostfixExpression, " ");
                }

                LLS_DestroyNode(Popped);
            }

            LLS_Push(Stack, LLS_CreateNode(Token)); // 토큰을 스택에 푸시
        }
    }

    // 6
    while (!LLS_IsEmpty(Stack)) // Stack의 연산자를 PostfixExpression에 이어붙임
    {
        Node *Popped = LLS_Pop(Stack);
        
        if (Popped->Data[0] != LEFT_PARENTHESIS)
        {
            strcat(PostfixExpression, Popped->Data);
            strcat(PostfixExpression, " ");
        }

        LLS_DestroyNode(Popped);
    }

    LLS_DestroyStack(Stack);
}

// 후위표현식을 연산하여 결과 반환
double Calculate(char *PostfixExpression)
{
    LinkedListStack *Stack;
    Node* ResultNode;
    double Result;
    char Token[32];
    int Type = -1;
    unsigned int Read = 0;
    unsigned int Length = strlen(PostfixExpression);

    LLS_CreateStack(&Stack);

    while (Read < Length)
    {
        Read += GetNextToken(&PostfixExpression[Read], Token, &Type);

        if (Type == SPACE)
        {
            continue;
        }

        if (Type == OPERAND)
        {
            Node *NewNode = LLS_CreateNode(Token);
            LLS_Push(Stack, NewNode);
        }
        else
        {
            char ResultString[32];
            double Operator1, Operator2, TempResult;
            Node *OperatorNode;

            OperatorNode = LLS_Pop(Stack);
            Operator2 = atof(OperatorNode->Data);
            LLS_DestroyNode(OperatorNode);

            OperatorNode = LLS_Pop(Stack);
            Operator1 = atof(OperatorNode->Data);
            LLS_DestroyNode(OperatorNode);

            switch (Type)
            {
                case PLUS:      TempResult = Operator1 + Operator2; break;
                case MINUS:     TempResult = Operator1 - Operator2; break;
                case MULTIPLY:  TempResult = Operator1 * Operator2; break;
                case DIVIDE:    TempResult = Operator1 / Operator2; break;
            }

            gcvt(TempResult, 10, ResultString);
            LLS_Push(Stack, LLS_CreateNode(ResultString));
        }
    }

    ResultNode = LLS_Pop(Stack);
    Result = atof(ResultNode->Data);
    LLS_DestroyNode(ResultNode);

    LLS_DestroyStack(Stack);

    return Result;
}

// 메인
int main(int argc, char **argv)
{
    char InfixExpression[128];
    char PostfixExpression[128];
    double Result = 0.0;

    memset(InfixExpression, 0, sizeof(InfixExpression));
    memset(PostfixExpression, 0, sizeof(PostfixExpression));

    memcpy(InfixExpression, argv[1], strlen(argv[1]));

    GetPostfix(InfixExpression, PostfixExpression);

    fprintf(stdout, "Infix:%s\nPostfix:%s\n", InfixExpression, PostfixExpression);
    
    Result = Calculate(PostfixExpression);

    fprintf(stdout, "Calculation Result : %lf\n", Result);

    return 0;
}

/*
[ GetPostfixExample ]

> Infix : "1+3.334/(4.28*(110-7729))"

01) PostFix : 1
    Stack   :

02) PostFix : 1
    Stack   : +

03) PostFix : 1 3.334
    Stack   : +

04) PostFix : 1 3.334
    Stack   : + /

05) PostFix : 1 3.334
    Stack   : + / (

06) PostFix : 1 3.334 4.28
    Stack   : + / (

07) PostFIx : 1 3.334 4.28
    Stack   : + / ( *

08) PostFIx : 1 3.334 4.28
    Stack   : + / ( * (

09) PostFIx : 1 3.334 4.28 110
    Stack   : + / ( * (

10) PostFIx : 1 3.334 4.28 110
    Stack   : + / ( * ( -

11) PostFIx : 1 3.334 4.28 110 7729
    Stack   : + / ( * ( -

12) PostFIx : 1 3.334 4.28 110 7729 -
    Stack   : + / ( *

13) PostFIx : 1 3.334 4.28 110 7729 - *
    Stack   : + /

14) PostFIx : 1 3.334 4.28 110 7729 - * / +
    Stack   : 

> PostFix : "1 3.334 4.28 110 7729 - * / +"

[ Calculation ]

1) PostFix : * / +
   Stack   : 1 3.334 4.28 {110 7729 -}
   Result  : 110 7729 -

2) PostFix : / +
   Stack   : 1 3.334 {4.28 -7619.00 *}
   Result  : 4.28 -7619.00 *

3) PostFix : +
   Stack   : 1 {3.334 -32609.32 /}
   Result  : 3.334 -32609.32 /

4) PostFix :
   Stack   : {1 -0.0001022407 +}
   Result  : 1 -0.0001022407 +

> Result : 0.999898

*/