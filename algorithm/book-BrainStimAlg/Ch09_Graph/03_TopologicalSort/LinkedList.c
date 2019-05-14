#include "LinkedList.h"

Node* SLL_CreateNode(Vertex* NewData)
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