#include "LinkedList.h"

Node* SLL_CreateNode(ElementType NewData)
{
    Node* NewNode = (Node*)malloc(sizeof(Node));

    NewNode->Data = NewData;
    NewNode->NextNode = NULL;

    return NewNode;
}

void SLL_DestroyNode(Node* Node)
{
    free(Node);
}

void SLL_DestroyAllNodes(Node** List)
{
    if ( (*List) == NULL )
    {
        return;
    }
    else
    {
        Node* Current = (*List);
        Node* Next    = NULL;
        while ( Current != NULL )
        {
            Next = Current->NextNode;
            Current->NextNode = NULL;
            free(Current);
            (*List) = NULL;
            Current = Next;
        }
    }
}

void SLL_AppendNode(Node** Head, Node* NewNode)
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
    }
}

void SLL_InsertAfter(Node* Current, Node* NewNode)
{
    NewNode->NextNode = Current->NextNode;
    Current->NextNode = NewNode;
}

void SLL_InsertNewHead(Node** Head, Node* NewHead)
{
    if ( (*Head) == NULL )
    {
        (*Head) = NewHead;
    }
    else
    {
        NewHead->NextNode = (*Head);
        (*Head) = NewHead;
    }
}

void SLL_RemoveNode(Node** Head, Node* Remove)
{
    if ( (*Head) == NULL )
    {
        *Head = Remove->NextNode;
    }
    else
    {
        Node* Current = *Head;
        while ( Current != NULL && Current->NextNode != Remove )
        {
            Current = Current->NextNode;
        }

        if ( Current != NULL )
        {
            Current->NextNode = Remove->NextNode;
        }
    }
}

Node* SLL_GetNodeAt(Node* Head, int Location)
{
    Node* Current = Head;

    while ( Current != NULL && (--Location) >= 0 )
    {
        Current = Current->NextNode;
    }

    return Current;
}

int SLL_GetNodeCount(Node* Head)
{
    int     Count = 0;
    Node*   Current = Head;

    while ( Current != NULL )
    {
        Current = Current->NextNode;
        ++Count;
    }

    return Count;
}

static int SLL_Test_main(void)
{
    int     i       = 0;
    int     Count   = 0;
    Node*   List    = NULL;
    Node*   Current = NULL;
    Node*   NewNode = NULL;

    for ( i = 0; i < 5; ++i )
    {
        NewNode = SLL_CreateNode(i);
        SLL_AppendNode(&List, NewNode);
    }

    NewNode = SLL_CreateNode(-1);
    SLL_InsertNewHead(&List, NewNode);

    NewNode = SLL_CreateNode(-2);
    SLL_InsertNewHead(&List, NewNode);

    Count = SLL_GetNodeCount(List);
    for ( i = 0; i < Count; ++i )
    {
        Current = SLL_GetNodeAt(List, i);
        fprintf(stdout, "List[%d] : %d\n", i, Current->Data);
    }

    fprintf(stdout, "\nInserting 3000 After [2]...\n\n");

    Current = SLL_GetNodeAt(List, 2);
    NewNode = SLL_CreateNode(3000);
    SLL_InsertAfter(Current, NewNode);

    Count = SLL_GetNodeCount(List);
    for ( i = 0; i < Count; ++i )
    {
        Current = SLL_GetNodeAt(List, i);
        fprintf(stdout, "List[%d] : %d\n", i, Current->Data);
    }

    fprintf(stdout, "\nDestrying List...\n");

    SLL_DestroyAllNodes(&List);
    
/*    for ( i = 0; i < Count; ++i )  // Almost same with 'SLL_DestroyAllNodes()'
    {
        Current = SLL_GetNodeAt(List, 0);

        if ( Current != NULL )
        {
            SLL_RemoveNode(&List, Current);
            SLL_DestroyNode(Current);
        }
    }
*/
    Count = SLL_GetNodeCount(List);

    fprintf(stdout, "\nListCount : %d\n", Count);

    return 0;
}

int main(int argc, char **argv)
{
    return SLL_Test_main();
}