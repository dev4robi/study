#include "DoublyLinkedList.h"

Node* DLL_CreateNode(ElementType NewData)
{
    Node* NewNode = (Node*)malloc(sizeof(Node));

    NewNode->Data     = NewData;
    NewNode->PrevNode = NULL;
    NewNode->NextNode = NULL;

    return NewNode;
}

void DLL_DestroyNode(Node* Node)
{
    if ( Node != NULL )
        free(Node);
}

void DLL_AppendNode(Node** Head, Node* NewNode)
{
    if ( (*Head) == NULL )
    {
        *Head = NewNode;
    }
    else
    {
        Node* Tail = (*Head);
        while ( Tail->NextNode != NULL )
        {
            Tail = Tail->NextNode;
        }

        Tail->NextNode = NewNode;
        NewNode->PrevNode = Tail;
    }
}

void DLL_InsertAfter(Node* Current, Node* NewNode)
{
    NewNode->NextNode = Current->NextNode;
    NewNode->PrevNode = Current;

    if ( Current->NextNode != NULL )
    {
        Current->NextNode->PrevNode = NewNode;
    }

    Current->NextNode = NewNode;
}

void DLL_RemoveNode(Node** Head, Node* Remove)
{
    if ( (*Head) == Remove )
    {
        *Head = Remove->NextNode;
        if ( (*Head) != NULL )
            (*Head)->PrevNode = NULL;

        Remove->PrevNode = NULL;
        Remove->NextNode = NULL;
    }
    else
    {
        Node* Temp = Remove;

        if ( Remove->PrevNode != NULL )
            Remove->PrevNode->NextNode = Temp->NextNode;

        if ( Remove->NextNode != NULL )
            Remove->NextNode->PrevNode = Temp->PrevNode;

        Remove->PrevNode = NULL;
        Remove->NextNode = NULL;
    }
}

Node* DLL_GetNodeAt(Node* Head, int Location)
{
    Node* Current = Head;

    while ( Current != NULL && (--Location) >= 0 )
    {
        Current = Current->NextNode;
    }

    return Current;
}

int DLL_GetNodeCount(Node* Head)
{
    unsigned int    Count = 0;
    Node*           Current = Head;

    while ( Current != NULL )
    {
        Current = Current->NextNode;
        ++Count;
    }

    return Count;
}

void DLL_PrintReverse(Node* Head)
{
    int     Count = 0;
    Node*   Tail = NULL;

    Count = DLL_GetNodeCount(Head) - 1;
    Tail = DLL_GetNodeAt(Head, Count);
    while ( Tail != NULL )
    {
        fprintf(stdout, "List[%d] : %d\n", Count, Tail->Data);
        Tail = Tail->PrevNode;
        --Count;
    }
}

static int DLL_Test_main()
{
    int     i       = 0;
    int     Count   = 0;
    Node*   List    = NULL;
    Node*   NewNode = NULL;
    Node*   Current = NULL;

    for ( i = 0; i < 5; ++i )
    {
        NewNode = DLL_CreateNode(i);
        DLL_AppendNode(&List, NewNode);
    }

    Count = DLL_GetNodeCount(List);
    for ( i = 0; i < Count; ++i )
    {
        Current = DLL_GetNodeAt(List, i);
        fprintf(stdout, "List[%d] : %d\n", i, Current->Data);
    }

    fprintf(stdout, "\nInserting 3000 After [2]...\n\n");

    Current = DLL_GetNodeAt(List, 2);
    NewNode = DLL_CreateNode(3000);
    DLL_InsertAfter(Current, NewNode);

    Count = DLL_GetNodeCount(List);
    for ( i = 0; i < Count; ++i )
    {
        Current = DLL_GetNodeAt(List, i);
        fprintf(stdout, "List[%d] : %d\n", i, Current->Data);
    }

    fprintf(stdout, "\nPrint Reverse...\n\n");

    DLL_PrintReverse(List);

    printf("\nDestorying List...\n");

    Count = DLL_GetNodeCount(List);
    for ( i = 0; i < Count; ++i )
    {
        Current = DLL_GetNodeAt(List, 0);

        if ( Current != NULL )
        {
            DLL_RemoveNode(&List, Current);
            DLL_DestroyNode(Current);
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    return DLL_Test_main();
}