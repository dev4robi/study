#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

/*
    [ Disjoint Set : 분리 집합 ]
     - 교집합이 없는 두개 이상의 집합을 의미
     - 집합 자체를 트리의 Root노드로, 집합의 원소들은 트리를 Pointing하는 Child노드로 사용
     - 다른 트리와 다르게 부모에서 자식으로 Pointing하지 않는다
     - 두 집합 A, B의 합집합은 B집합의 Root노드를 A집합의 Child노드로 두고, A의 Root를 Pointing시킴
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct tagDisjointSet
{
    struct tagDisjointSet*  Parent;
    void*                   Data;
} DisjointSet;

void            DS_UnionSet(DisjointSet* Set1, DisjointSet* Set2);
DisjointSet*    DS_FindSet(DisjointSet* Set);
DisjointSet*    DS_MakeSet(void* NewData);
void            DS_DestroySet(DisjointSet* Set);

#endif