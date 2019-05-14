#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

typedef struct tagNode
{
    Vertex* Data;
    struct tagNode* NextNode;
} Node;

/* Single Linked List (SLL) */
Node*   SLL_CreateNode(Vertex* NewData);
void    SLL_DestroyNode(Node* Node);
void    SLL_DestroyAllNodes(Node** List);
void    SLL_AppendNode(Node** Head, Node* NewNode);
void    SLL_InsertAfter(Node* Current, Node* NewNode);
void    SLL_InsertBefore(Node** Head, Node* Current, Node* NewNode);
void    SLL_InsertNewHead(Node** Head, Node* NewHead);
Node*   SLL_GetNodeAt(Node* Head, int Location);
int     SLL_GetNodeCount(Node* Head);

#endif