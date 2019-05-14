#include "CircularDoublyLinkedList.h"

Node* CDLL_CreateNode(ElementType NewData)
{
    Node* NewNode = (Node*)malloc(sizeof(Node));

    NewNode->Data     = NewData;
    NewNode->PrevNode = NULL;
    NewNode->NextNode = NULL;

    return NewNode;
}

void CDLL_DestroyNode(Node* Node)
{
    if ( Node != NULL )
        free(Node);
}

void CDLL_AppendNode(Node** Head, Node* NewNode)
{
    if ( (*Head) == NULL )
    {
        *Head = NewNode;
        (*Head)->PrevNode = *Head;
        (*Head)->NextNode = *Head;
    }
    else
    {
        Node* Tail = (*Head)->PrevNode;

        Tail->NextNode->PrevNode = NewNode;
        Tail->NextNode = NewNode;

        NewNode->NextNode = (*Head);
    }
}

void CDLL_InsertAfter(Node* Current, Node* NewNode)
{
    NewNode->NextNode = Current->NextNode;
    NewNode->PrevNode = Current;

    if ( Current->NextNode != NULL )
    {
        Current->NextNode->PrevNode = NewNode;
    }

    Current->NextNode = NewNode;
}

void CDLL_RemoveNode(Node** Head, Node* Remove)
{
    if ( (*Head) == Remove )
    {
        (*Head)->PrevNode->NextNode = Remove->NextNode;
        (*Head)->NextNode->PrevNode = Remove->PrevNode;

        *Head = Remove->NextNode;

        Remove->PrevNode = NULL;
        Remove->NextNode = NULL;
    }
    else
    {
        Node* Temp = Remove;

        Remove->PrevNode->NextNode = Temp->NextNode;
        Remove->NextNode->PrevNode = Temp->PrevNode;

        Remove->PrevNode = NULL;
        Remove->NextNode = NULL;
    }
}

Node* CDLL_GetNodeAt(Node* Head, int Location)
{
    Node* Current = Head;

    while ( Current != NULL && (--Location) >= 0 )
    {
        Current = Current->NextNode;
    }

    return Current;
}

int CDLL_GetNodeCount(Node* Head)
{
    unsigned int    Count = 0;
    Node*           Current = Head;

    while ( Current != NULL )
    {
        Current = Current->NextNode;
        ++Count;

        if ( Current == Head )
            break;
    }

    return Count;
}

void CDLL_PrintReverse(Node* Head)
{
    int     Count = 0;
    Node*   Tail = NULL;

    Count = CDLL_GetNodeCount(Head) - 1;
    Tail = CDLL_GetNodeAt(Head, Count);
    while ( Tail != NULL )
    {
        fprintf(stdout, "List[%d] : %d\n", Count, Tail->Data);
        Tail = Tail->PrevNode;
        --Count;
    }
}

static int CDLL_Test_main()
{
    int     i       = 0;
    int     Count   = 0;
    Node*   List    = NULL;
    Node*   NewNode = NULL;
    Node*   Current = NULL;

    for ( i = 0; i < 5; ++i )
    {
        NewNode = CDLL_CreateNode(i);
        CDLL_AppendNode(&List, NewNode);
    }

    Count = CDLL_GetNodeCount(List);
    for ( i = 0; i < Count; ++i )
    {
        Current = CDLL_GetNodeAt(List, i);
        fprintf(stdout, "List[%d] : %d\n", i, Current->Data);
    }

    fprintf(stdout, "\nInserting 3000 After [2]...\n\n");

    Current = CDLL_GetNodeAt(List, 2);
    NewNode = CDLL_CreateNode(3000);
    CDLL_InsertAfter(Current, NewNode);

    Count = CDLL_GetNodeCount(List) * 2; // Check 'Circular' List
    for ( i = 0; i < Count; ++i )
    {
        if (i == 0)
            Current = List;
        else
            Current = Current->NextNode;

        fprintf(stdout, "List[%d] : %d\n", i, Current->Data);
    }

    fprintf(stdout, "\nPrint Reverse...\n\n");

    CDLL_PrintReverse(List);

    printf("\nDestorying List...\n");

    Count = CDLL_GetNodeCount(List);
    for ( i = 0; i < Count; ++i )
    {
        Current = CDLL_GetNodeAt(List, 0);

        if ( Current != NULL )
        {
            CDLL_RemoveNode(&List, Current);
            CDLL_DestroyNode(Current);
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    return CDLL_Test_main();
}