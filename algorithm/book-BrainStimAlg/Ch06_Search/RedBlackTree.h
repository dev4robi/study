#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <stdio.h>
#include <stdlib.h>

/*
    [ 규칙1. 레드블랙 트리 ]
    1. 모든 노드는 {빨간색} 아니면 (검은색) 이다.
    2. 루트 노드는 (검은색) 이다.
    3. 잎 노드는 (검은색) 이다.
    4. "{빨간색} 노드의 자식들은 모두 (검은색) 이다." 하지만, (검은색) 노드의 자식들은 {빨간색} 일 필요는 없다.
    5. 루트 노드와 모든 잎 노드 사이에 있는 (검은색) 노드의 수는 모두 동일하다.


    [ 규칙2. 레드블랙 트리 로테이션 ]
    1. 트리 우회전을 할 때는 왼쪽 자식 노드의 오른쪽 자식 노드를 부모 노드의 왼쪽 자식으로 연결한다.
    2. 트리 좌회전을 할 때는 오른쪽 자식 노두의 왼쪽 자식 노드를 부모 노드의 오른쪽 자식으로 연결한다.

          (8)                (5)
         /   \     =R>      /   \
       (5)   {9}          {3}   (8)
      /   \        <L=         /   \
    {3}   {6}                {6}   {9}


    [ 규칙3. 노드 삽입 후 재정렬 ]
    + 현재 기준은 부모 노드가 할아버지의 왼쪽 자식인 경우임. 오른쪽 자식의 경우 아래 설명의 오른쪽 왼쪽을 반대로 변경.
    + 새로 삽입된 노드는 항상 빨간색이다.
    1. 삽입한 노드의 삼촌 노드(삽입노드->부모->할아버지->다른부모) 가 빨간색인 경우
        - 부모 노드와 삼촌 노드를 검은색으로 칠하고 할아버지를 빨간색으로 칠한다.
          (부모 노드가 검은색이거나, 루트노드 일 때 까지 할아버지 노드를 삽입했다고 가정하고 재귀)
    2. 삽입한 노드의 삼촌 노드가 검은색이고 부모 노드의 오른쪽으로 삽입된 경우
        - 부모 노드를 왼쪽으로 회전시키고, 기존의 부모 노드를 새로 삽입된 노드로 치고 3번을 적용한다.
    3. 삽입한 노드의 삼촌 노드가 검은색이고 부모 노드의 왼쪽으로 삽입된 경우
        - 부모 노드를 검은색, 할아버지를 빨산색으로 칠한다하고 할아버지 노드를 오른쪽으로 회전시킨다.

    
    [ 규칙4. 노드 삭제 후 재정렬 ]
    + 삭제한 노드가 빨간색 노드라면 다른 뒷처리를 할 필요가 없다. 아래 설명은 삭제한 노드가 검은색이면 수행할 행동들이다.
    + 루트가 이중흑색 노드가 되면 검은색으로 칠한다.
    1. 삭제한 노드를 대체할 노드가 빨간색인 경우
        - 대체할 노드를 검은색으로 칠한다.
    2. 삭제한 노드를 대체할 노드가 검은색인 경우
        - 대체할 노드를 검은색으로 또 칠한다. (이중흑색 노드가 된다)
    3. 이중흑색 노드를 처리하는 방법 (이중흑색 노드가 부모의 왼쪽 자식인 경우. 오른쪽의 경우는 왼쪽 오른쪽만 변경.)
        1) 이중흑색 노드의 형제 노드가 빨간색인 경우
            - 형제 노드를 검은색, 부모 노드를 빨간색으로 칠하고 부모를 기준으로 좌회전한다.
              이후 2)의 해법을 적용할 수 있게 된다.
        2) 이중흑색 노드의 형제 노드가 검은색인 경우
            (1) 이중흑색 노드의 형제가 검은색이고 형제의 양쪽 자식이 모두 검은색인 경우
                - 형제 노드를 빨산색으로 칠하고 이중흑색 노드의 검은색 하나를 부모 노드에게 넘겨줌. (부모와 이중흑색 노드가 검은색이 됨)
            (2) 이중흑색 노드의 형제가 검은색이고 형제의 왼쪽 자식은 빨간색, 오른쪽 자식은 검은색인 경우
                - 형제의 노드를 빨간색으로 칠하고 왼쪽 자식을 검은색으로 칠하고 형제노드를 기준으로 우회전을 수행.
                  이후 (3)을 수행할 수 있게 된다.
            (3) 형제가 검은색이고 형제의 오른쪽 자식이 빨간색인 경우
                - 이중흑색 노드의 부모 노드가 갖고 있는 색을 형제 노드에 칠하고, 부모 노드와 형제 노드의 오른쪽 자식 노드를 검은색으로 칠함.
                  이후 부모 노드를 기준으로 좌회전하고, 이중흑색을 할아버지 노드에게 넘긴다. (재귀 반복)
*/

typedef int ElementType;

typedef struct tagRBTNode
{
    struct tagRBTNode* Parent;
    struct tagRBTNode* Left;
    struct tagRBTNode* Right;
    ElementType Data;
    enum { RED, BLACK } Color;

} RBTNode;

void        RBT_DestroyTree(RBTNode* Tree);

RBTNode*    RBT_CreateNode(ElementType NewData);
void        RBT_DestroyNode(RBTNode* Node);

RBTNode*    RBT_SearchNode(RBTNode* Tree, ElementType Target);
RBTNode*    RBT_SearchMinNode(RBTNode* Tree);
void        RBT_InsertNode(RBTNode** Tree, RBTNode *NewNode);
void        RBT_InsertNodeHelper(RBTNode** Tree, RBTNode* NewNode);
RBTNode*    RBT_RemoveNode(RBTNode** Root, ElementType Data);
void        RBT_RebuildAfterInsert(RBTNode** Root, RBTNode* X);
void        RBT_RebuildAfterRemove(RBTNode** Root, RBTNode* Successor);

void        RBT_PrintTree(RBTNode* Node, int Depth, int BlackCount);
void        RBT_RotateLeft(RBTNode** Root, RBTNode* Parent);
void        RBT_RotateRight(RBTNode** Root, RBTNode* Parent);

#endif