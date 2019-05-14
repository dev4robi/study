#include "ExpressionTree.h"

ETNode* ET_CreateNode(ElementType NewData)
{
    ETNode* NewNode = (ETNode*)malloc(sizeof(ETNode));
    NewNode->Left = NULL;
    NewNode->Right = NULL;
    NewNode->Data = (ElementType)malloc(strlen(NewData) + 1);
    strcpy(NewNode->Data, NewData);

    return NewNode;
}

void ET_DestroyNode(ETNode* Node)
{
    if (Node != NULL)
    {
        if (Node->Data != NULL)
        {
            memset(Node->Data, 0x00, strlen(Node->Data));
            free (Node->Data);
        }

        Node->Left = NULL;
        Node->Right = NULL;
        Node->Data = NULL;
        free(Node);
    }
}

void ET_DestroyTree(ETNode* Root)
{
    if (Root == NULL) return;

    ET_DestroyTree(Root->Left);
    ET_DestroyTree(Root->Right);
    ET_DestroyNode(Root);
}

void ET_PreorderPrintTree(ETNode* Node)
{
    if (Node == NULL) return;

    fprintf(stdout, " %s", Node->Data);
    ET_PreorderPrintTree(Node->Left);
    ET_PreorderPrintTree(Node->Right);
}

void ET_InorderPrintTree(ETNode* Node)
{
    if (Node == NULL) return;

    fprintf(stdout, "(");
    ET_InorderPrintTree(Node->Left);
    fprintf(stdout, "%s", Node->Data);
    ET_InorderPrintTree(Node->Right);
    fprintf(stdout, ")");
}

void ET_PostorderPrintTree(ETNode* Node)
{
    if (Node == NULL) return;

    ET_PostorderPrintTree(Node->Left);
    ET_PostorderPrintTree(Node->Right);
    fprintf(stdout, " %s", Node->Data);
}

void ET_BuildExpressionTree(char* PostfixExpression, ETNode** Node)
{
    int len = strlen(PostfixExpression);
    char Token[255];

    memset(Token, 0x00, sizeof(Token));

    do
    {
        --len;

        if (len < 0)
        {
            break;
        }

        Token[0] = PostfixExpression[len];
        PostfixExpression[len] = '\0';
    } while (Token[0] == ' ');

    switch (Token[0])
    {
        case '+': case '-': case '*': case '/':
        {
            (*Node) = ET_CreateNode(&Token[0]);
            ET_BuildExpressionTree(PostfixExpression, &(*Node)->Right);
            ET_BuildExpressionTree(PostfixExpression, &(*Node)->Left);
            break;
        }
        default:
        {
            int i = len - 1, j = 1;

            // Number tokenize
            for (; i > -1; --i)
            {
                char c = PostfixExpression[i];

                fprintf(stdout, "pe: %s\n", PostfixExpression);
                
                if (c < '0' || c > '9')
                {
                    PostfixExpression[i] = c;
                    break;
                }

                PostfixExpression[i] = '\0';
                Token[j] = c;
                ++j;
            }
            // Swap buffer
            {
                char *pN1 = &Token[0];
                char *pN2 = &Token[strlen(Token) - 1];
                
                while (pN1 < pN2)
                {
                    char temp = *pN1;

                    *pN1 = *pN2;
                    *pN2 = temp;
                    ++pN1;
                    --pN2;
                }
            }

            (*Node) = ET_CreateNode(Token);
            break;
        }
    }
}

double ET_Evaluate(ETNode* Tree)
{
    double Result = 0.0;

    if (Tree == NULL) return 0.0;

    switch (Tree->Data[0])
    {
        case '+': case '-': case '*': case '/':
        {
            double Left  = ET_Evaluate(Tree->Left);
            double Right = ET_Evaluate(Tree->Right);

            if      (Tree->Data[0] == '+') Result = Left + Right;
            else if (Tree->Data[0] == '-') Result = Left - Right;
            else if (Tree->Data[0] == '*') Result = Left * Right;
            else if (Tree->Data[0] == '/') Result = Left / Right;

            break;
        }
        default:
        {
            Result = (double)atof(Tree->Data);
            break;
        }
    }

    return Result;
}

int ET_Test_main()
{
    ETNode* Root = NULL;
    char PostfixExpression[20] = "70 11 * 5 2 - /";

    ET_BuildExpressionTree(PostfixExpression, &Root);

    fprintf(stdout, "Preorder...\n");
    ET_PreorderPrintTree(Root);
    fprintf(stdout, "\n\n");

    fprintf(stdout, "Inorder...\n");
    ET_InorderPrintTree(Root);
    fprintf(stdout, "\n\n");

    fprintf(stdout, "Postorder...\n");
    ET_PostorderPrintTree(Root);
    fprintf(stdout, "\n\n");

    fprintf(stdout, "Evaluation Result : %lf\n", ET_Evaluate(Root));

    ET_DestroyTree(Root);

    return 0;
}

int main()
{
    return ET_Test_main();
}