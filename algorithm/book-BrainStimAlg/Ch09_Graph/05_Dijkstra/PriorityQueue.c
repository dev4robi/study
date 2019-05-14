#include "PriorityQueue.h"

PriorityQueue* PQ_Create(int InitialSize)
{
    PriorityQueue* NewPQ = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    NewPQ->Capacity = InitialSize;
    NewPQ->UsedSize = 0;
    NewPQ->Nodes = (PQNode*)malloc(sizeof(PQNode) * NewPQ->Capacity);

    return NewPQ;
}

void PQ_Destroy(PriorityQueue* PQ)
{
    if (PQ != NULL)
    {
        if (PQ->Nodes != NULL) free(PQ->Nodes);

        free (PQ);
    }
}

void PQ_Enqueue(PriorityQueue* PQ, PQNode NewNode)
{
    int CurrentPos = PQ->UsedSize;
    int ParentPos = PQ_GetParent(CurrentPos);

    if (PQ->UsedSize == PQ->Capacity)
    {
        if (PQ->Capacity == 0)
            PQ->Capacity = 1;

        PQ->Capacity *= 2;
        PQ->Nodes = (PQNode*)realloc(PQ->Nodes, sizeof(PQNode) * PQ->Capacity);
    }

    PQ->Nodes[CurrentPos] = NewNode;

    while (CurrentPos > 0 && PQ->Nodes[CurrentPos].Priority < PQ->Nodes[ParentPos].Priority)
    {
        PQ_SwapNodes(PQ, CurrentPos, ParentPos);
        CurrentPos = ParentPos;
        ParentPos = PQ_GetParent(CurrentPos);
    }

    ++(PQ->UsedSize);
}

void PQ_Dequeue(PriorityQueue* PQ, PQNode* Root)
{
    int ParentPos = 0;
    int LeftPos = 0;
    int RightPos = 0;

    memcpy(Root, &PQ->Nodes[0], sizeof(PQNode));
    memset(&PQ->Nodes[0], 0, sizeof(PQNode));

    --(PQ->UsedSize);
    PQ_SwapNodes(PQ, 0, PQ->UsedSize);

    LeftPos = PQ_GetLeftChild(0);
    RightPos = LeftPos + 1;

    while (1)
    {
        int SelectedChild = 0;

        if (LeftPos >= PQ->UsedSize) break;

        if (RightPos >= PQ->UsedSize)
        {
            SelectedChild = LeftPos;
        }
        else
        {
            if (PQ->Nodes[LeftPos].Priority > PQ->Nodes[RightPos].Priority)
                SelectedChild = RightPos;
            else
                SelectedChild = LeftPos;
        }

        if (PQ->Nodes[SelectedChild].Priority < PQ->Nodes[ParentPos].Priority)
        {
            PQ_SwapNodes(PQ, ParentPos, SelectedChild);
            ParentPos = SelectedChild;
        }
        else
            break;

        LeftPos = PQ_GetLeftChild(ParentPos);
        RightPos = LeftPos + 1;
    }

    if (PQ->UsedSize < (PQ->Capacity / 2))
    {
        PQ->Capacity /= 2;
        PQ->Nodes = (PQNode*)realloc(PQ->Nodes, sizeof(PQNode) * PQ->Capacity);
    }
}

int PQ_GetParent(int Index)
{
    return (int)((Index - 1) / 2);
}

int PQ_GetLeftChild(int Index)
{
    return (2 * Index) + 1;
}

void PQ_SwapNodes(PriorityQueue* PQ, int Index1, int Index2)
{
    int CopySize = sizeof(PQNode);
    PQNode* Temp = (PQNode*)malloc(CopySize);

    memcpy(Temp,                &PQ->Nodes[Index1], CopySize);
    memcpy(&PQ->Nodes[Index1],  &PQ->Nodes[Index2], CopySize);
    memcpy(&PQ->Nodes[Index2],  Temp,               CopySize);

    if (Temp != NULL) free(Temp);
}

int PQ_IsEmpty(PriorityQueue* PQ)
{
    return (PQ->UsedSize == 0);
}