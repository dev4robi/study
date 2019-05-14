#include "LinkedListStack.h"

void LLS_CreateStack(LinkedListStack** Stack)
{
    (*Stack)        = (LinkedListStack*)malloc(sizeof(LinkedListStack));
    (*Stack)->List  = NULL;
    (*Stack)->Top   = NULL;
}

void LLS_DestroyStack(LinkedListStack* Stack)
{
    if (Stack != NULL)
    {
        while (!LLS_IsEmpty(Stack))
        {
            Node* Popped = LLS_Pop(Stack);
            LLS_DestroyNode(Popped);
        }

        free(Stack);
    }
}

Node* LLS_CreateNode(char* NewData)
{
    Node* NewNode = (Node*)malloc(sizeof(Node));

    NewNode->Data = (char*)malloc(strlen(NewData) + 1);
    strcpy(NewNode->Data, NewData);
    NewNode->NextNode = NULL;
    
    return NewNode;
}

void LLS_DestroyNode(Node* _Node)
{
    if (_Node != NULL)
    {
        free(_Node->Data);
        free(_Node);
    }
}

void LLS_Push(LinkedListStack* Stack, Node* NewNode)
{
    if (Stack->List == NULL)
    {
        Stack->List = NewNode;
    }
    else
    {
        Node* OldTop = Stack->List;
        while (OldTop->NextNode != NULL)
        {
            OldTop = OldTop->NextNode;
        }

        OldTop->NextNode = NewNode;
    }

    Stack->Top = NewNode;
}

Node* LLS_Pop(LinkedListStack* Stack)
{
    Node* TopNode = Stack->Top;

    if (Stack->List == Stack->Top)
    {
        Stack->List = NULL;
        Stack->Top  = NULL;
    }
    else
    {
        Node* CurrentTop = Stack->List;
        while (CurrentTop != NULL && CurrentTop->NextNode != Stack->Top)
        {
            CurrentTop = CurrentTop->NextNode;
        }

        Stack->Top = CurrentTop;
        CurrentTop->NextNode = NULL;
    }

    return TopNode;
}

Node* LLS_Top(LinkedListStack* Stack)
{
    return Stack->Top;
}

int LLS_GetSize(LinkedListStack* Stack)
{
    int     Count = 0;
    Node*   Current = Stack->List;

    while (Current != NULL)
    {
        Current = Current->NextNode;
        ++Count;
    }

    return Count;
}

int LLS_IsEmpty(LinkedListStack* Stack)
{
    return (Stack->List == NULL);
}

static int LLS_Test_main()
{
    int i = 0;
    int Count = 0;
    Node* Popped;
    LinkedListStack* Stack;

    LLS_CreateStack(&Stack);

    LLS_Push(Stack, LLS_CreateNode("abc"));
    LLS_Push(Stack, LLS_CreateNode("def"));
    LLS_Push(Stack, LLS_CreateNode("efg"));
    LLS_Push(Stack, LLS_CreateNode("hij"));

    Count = LLS_GetSize(Stack);

    fprintf(stdout, "Size: %d, Top: %s\n\n", Count, LLS_Top(Stack)->Data);

    for (i = 0; i < Count; ++i)
    {
        if (LLS_IsEmpty(Stack))
            break;

        Popped = LLS_Pop(Stack);

        fprintf(stdout, "Popped: %s, ", Popped->Data);

        LLS_DestroyNode(Popped);

        if (!LLS_IsEmpty(Stack))
            fprintf(stdout, "Current Top: %s\n", LLS_Top(Stack)->Data);
        else
            fprintf(stdout, "Stack Is Empty.\n");
    }

    LLS_DestroyStack(Stack);

    return 0;
}

//int main(int argc, char** argv)
//{
//    return LLS_Test_main();
//}
