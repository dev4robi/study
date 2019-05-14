#ifndef SEQUENTIAL_SEARCH_H
#define SEQUENTIAL_SEARCH_H

#include <stdio.h>

typedef int DataType;

typedef struct tagNode
{
    struct tagNode *pNextNode;
    DataType Data;

} Node;

Node*   SSL_MoveToFront(Node** Head, int Target);               // Singly Linked List(SLL)에서 사용 가능한 알고리즘
int     Ary_MoveToFront(int Array[], int Length, int Target);   // Array(배열)에서 사용 가능한 알고리즘
Node*   SLL_Transpose(Node** Head, int Target);
int     Ary_TransposeAry(int Array[], int Length, int Target);

#endif