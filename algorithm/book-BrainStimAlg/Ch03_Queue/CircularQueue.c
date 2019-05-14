#include "CircularQueue.h"

void CQ_CreateQueue(CircularQueue** Queue, int Capacity)
{
    (*Queue)            = (CircularQueue*)malloc(sizeof(CircularQueue));
    (*Queue)->Nodes     = (Node*)malloc(sizeof(Node) * (Capacity + 1));
    (*Queue)->Capacity  = Capacity;
    (*Queue)->Front     = 0;
    (*Queue)->Rear      = 0;
}

void CQ_DestroyQueue(CircularQueue* Queue)
{
    free(Queue->Nodes);
    free(Queue);
}

void CQ_Enqueue(CircularQueue* Queue, ElementType Data)
{
    int Position = 0;

    if (Queue->Rear == Queue->Capacity)
    {
        Position = Queue->Rear;
        Queue->Rear = 0;
    }
    else
    {
        Position = Queue->Rear++;
    }

    Queue->Nodes[Position].Data = Data;
}

ElementType CQ_Dequeue(CircularQueue* Queue)
{
    int Position = Queue->Front;

    if (Queue->Front == Queue->Capacity)
    {
        Queue->Front = 0;
    }
    else
    {
        Queue->Front++;
    }

    return Queue->Nodes[Position].Data;
}

int CQ_GetSize(CircularQueue* Queue)
{
    if (Queue->Front <= Queue->Rear)
    {
        return Queue->Rear - Queue->Front;
    }
    else
    {
        return Queue->Rear + (Queue->Capacity - Queue->Front) + 1;
    }
}

int CQ_IsEmpty(CircularQueue* Queue)
{
    return (Queue->Front == Queue->Rear);
}

int CQ_IsFull(CircularQueue* Queue)
{
    if (Queue->Front < Queue->Rear)
    {
        return (Queue->Rear - Queue->Front) == Queue->Capacity;
    }
    else
    {
        return (Queue->Rear + 1) == Queue->Front;
    }
}

static int CQ_Test_main()
{
    int i;
    CircularQueue* Queue;

    CQ_CreateQueue(&Queue, 10);

    CQ_Enqueue(Queue, 1);
    CQ_Enqueue(Queue, 2);
    CQ_Enqueue(Queue, 3);
    CQ_Enqueue(Queue, 4);

    for (i = 0; i < 3; ++i)
    {
        fprintf(stdout, "Dequeue: %d, ", CQ_Dequeue(Queue));
        fprintf(stdout, "Front: %d, Rear: %d\n", Queue->Front, Queue->Rear);
    }

    i = 100;

    while (CQ_IsFull(Queue) == 0)
    {
        CQ_Enqueue(Queue, i++);
    }

    fprintf(stdout, "Capacity: %d, Size: %d\n\n", Queue->Capacity, CQ_GetSize(Queue));

    while (CQ_IsEmpty(Queue) == 0)
    {
        fprintf(stdout, "Dequeue: %d, ", CQ_Dequeue(Queue));
        fprintf(stdout, "Front: %d, Rear: %d\n", Queue->Front, Queue->Rear);
    }

    CQ_DestroyQueue(Queue);

    return 0;
}

int main()
{
    return CQ_Test_main();
}