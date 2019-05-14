#include "GraphTraversal.h"

// 깊이우선 탐색
void DFS(Vertex* V)
{
    Edge* E = NULL;

    if (V == NULL) return;

    fprintf(stdout, "%d ", V->Data);
    V->Visited = Visited;
    E = V->AdjacencyList;
    
    while (E != NULL) // 해당 정점에 인접한 다음 간선이 있음
    {
        if (E->Target != NULL && E->Target->Visited == NotVisited)
        {
            DFS(E->Target); // 재귀 호출
        }

        E = E->Next;
    }
}

// 너비우선 탐색
void BFS(Vertex* V, LinkedQueue* Queue)
{
    Edge* E = NULL;

    if (V == NULL) return;

    fprintf(stdout, "%d ", V->Data);
    V->Visited = Visited;

    LQ_Enqueue(Queue, LQ_CreateNode(V));

    while (!LQ_IsEmpty(Queue)) // 모든 연결된 정점을 다 탐색할 때 까지
    {
        Node* Popped = LQ_Dequeue(Queue);
        V = Popped->Data;
        E = V->AdjacencyList;

        while (E != NULL) // 정점의 모든 간선을 탐색할 때 까지
        {
            V = E->Target;

            if (V != NULL && V->Visited == NotVisited)
            {
                fprintf(stdout, "%d ", V->Data);
                V->Visited = Visited;
                LQ_Enqueue(Queue, LQ_CreateNode(V));
            }

            E = E->Next;
        }
    }
}

static int GT_Test_main()
{
    int     Mode = 0;
    Graph*  graph = CreateGraph();
    Vertex* V1 = CreateVertex(1);
    Vertex* V2 = CreateVertex(2);
    Vertex* V3 = CreateVertex(3);
    Vertex* V4 = CreateVertex(4);
    Vertex* V5 = CreateVertex(5);
    Vertex* V6 = CreateVertex(6);
    Vertex* V7 = CreateVertex(7);

    AddVertex(graph, V1);
    AddVertex(graph, V2);
    AddVertex(graph, V3);
    AddVertex(graph, V4);
    AddVertex(graph, V5);
    AddVertex(graph, V6);
    AddVertex(graph, V7);

    AddEdge(V1, CreateEdge(V1, V2, 0));
    AddEdge(V1, CreateEdge(V1, V3, 0));

    AddEdge(V2, CreateEdge(V2, V4, 0));
    AddEdge(V2, CreateEdge(V2, V5, 0));

    AddEdge(V3, CreateEdge(V3, V4, 0));
    AddEdge(V3, CreateEdge(V3, V6, 0));

    AddEdge(V4, CreateEdge(V4, V5, 0));
    AddEdge(V4, CreateEdge(V4, V7, 0));

    AddEdge(V5, CreateEdge(V5, V7, 0));
    
    AddEdge(V6, CreateEdge(V6, V7, 0));

    fprintf(stdout, "Enter Traversal Mode (0:DFS, 1:BFS) : ");
    fscanf(stdin, "%d", &Mode);

    if (Mode == 0)
    {
        DFS(graph->Vertices);
    }
    else
    {
        LinkedQueue* Queue = NULL;
        LQ_CreateQueue(&Queue);
        BFS(V1, Queue);
        LQ_DestroyQueue(Queue);
    }

    DestroyGraph(graph);

    return 0;
}

int main()
{
    return GT_Test_main();
}