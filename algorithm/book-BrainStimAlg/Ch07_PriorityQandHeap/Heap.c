#include "Heap.h"

Heap* HEAP_Create(int InitialSize)
{
    Heap* NewHeap = (Heap*)malloc(sizeof(Heap));
    NewHeap->Capacity = InitialSize;
    NewHeap->UsedSize = 0;
    NewHeap->Nodes = (HeapNode*)malloc(sizeof(HeapNode) * NewHeap->Capacity);

    return NewHeap;
}

void HEAP_Destroy(Heap* H)
{
    if (H != NULL)
    {
        if (H->Nodes != NULL) free(H->Nodes);

        free(H);
    }
}

void HEAP_Insert(Heap* H, ElementType NewData)
{
    int CurrentPos = H->UsedSize;
    int ParentPos = HEAP_GetParent(CurrentPos);

    if (H->UsedSize == H->Capacity) // Heap is full
    {
        H->Capacity *= 2;
        H->Nodes = (HeapNode*)realloc(H->Nodes, sizeof(HeapNode) * H->Capacity);
    }

    H->Nodes[CurrentPos].Data = NewData;

    while (CurrentPos > 0 && H->Nodes[CurrentPos].Data < H->Nodes[ParentPos].Data) // Swap insert node untill correct positon
    {
        HEAP_SwapNodes(H, CurrentPos, ParentPos);
        CurrentPos = ParentPos;
        ParentPos = HEAP_GetParent(CurrentPos);
    }

    ++(H->UsedSize);
}

void HEAP_DeleteMin(Heap* H, HeapNode* Root)
{
    int ParentPos = 0;
    int LeftPos = 0;
    int RightPos = 0;

    memcpy(Root, &H->Nodes[0], sizeof(HeapNode));
    memset(&H->Nodes[0], 0, sizeof(HeapNode));

    --(H->UsedSize);
    HEAP_SwapNodes(H, 0, H->UsedSize);

    LeftPos = HEAP_GetLeftChild(0);
    RightPos = LeftPos + 1;

    while (1)
    {
        int SelectedChild = 0;

        if (LeftPos >= H->UsedSize) break;

        if (RightPos >= H->UsedSize)
        {
            SelectedChild = LeftPos;
        }
        else
        {
            if (H->Nodes[SelectedChild].Data > H->Nodes[ParentPos].Data)
                SelectedChild = RightPos;
            else
                SelectedChild = LeftPos;
        }

        if (H->Nodes[SelectedChild].Data < H->Nodes[ParentPos].Data)
        {
            HEAP_SwapNodes(H, ParentPos, SelectedChild);
            ParentPos = SelectedChild;
        }
        else
            break;

        LeftPos = HEAP_GetLeftChild(ParentPos);
        RightPos = LeftPos + 1;
    }

    if (H->UsedSize < (H->Capacity / 2))
    {
        H->Capacity /= 2;
        H->Nodes = (HeapNode*)realloc(H->Nodes, sizeof(HeapNode) * H->Capacity);
    }
}

int HEAP_GetParent(int Index)
{
    return (int)((Index - 1) / 2);
}

int HEAP_GetLeftChild(int Index)
{
    return (2 * Index) + 1;
}

void HEAP_SwapNodes(Heap* H, int Index1, int Index2)
{
    int CopySize = sizeof(HeapNode);
    HeapNode* Temp = (HeapNode*)malloc(CopySize);

    memcpy(Temp,                &H->Nodes[Index1],  CopySize);
    memcpy(&H->Nodes[Index1],   &H->Nodes[Index2],  CopySize);
    memcpy(&H->Nodes[Index2],   Temp,               CopySize);

    free(Temp);
}

void HEAP_PrintNodes(Heap* H)
{
    int i = 0;

    for (i = 0; i < H->UsedSize; ++i)
    {
        fprintf(stdout, "%d ", H->Nodes[i].Data);
    }

    fprintf(stdout, "\n");
}

static int HEAP_Test_main()
{
    Heap* H = HEAP_Create(3);
    HeapNode MinNode;
    int datas[] = { 12, 87, 111, 34, 16, 75 };
    int szDatas = sizeof(datas) / sizeof(int);
    int i = 0;

    for (i = 0; i < szDatas; ++i)
    {
        fprintf(stdout, "+insert : ");
        HEAP_Insert(H, datas[i]);
        HEAP_PrintNodes(H);
    }

    for (i = 0; i < szDatas; ++i)
    {
        fprintf(stdout, "-delete : ");
        HEAP_DeleteMin(H, &MinNode);
        HEAP_PrintNodes(H);
    }

    return 0;
}

int main()
{
    return HEAP_Test_main();
}