#include "TopologicalSort.h"

void TopologicalSort(Vertex* V, Node** List)
{
    while (V != NULL && V->Visited == NotVisited)
    {
        TS_DFS(V, List);
        V = V->Next;
    }
}

void TS_DFS(Vertex* V, Node** List)
{
    Node* NewHead = NULL;
    Edge* E = NULL;

    V->Visited = Visited;
    E = V->AdjacencyList;

    while (E != NULL) // 특정 정점의 모든 간선을 조회하면서
    {
        if (E->Target != NULL && E->Target->Visited == NotVisited)
        {
            TS_DFS(E->Target, List); // 깊이우선 탐색 수행
        }

        E = E->Next;
    }

    fprintf(stdout, "%c\n", V->Data);
    NewHead = SLL_CreateNode(V);
    SLL_InsertNewHead(List, NewHead);
}

static int TS_Test_main()
{
    Node* SortedList = NULL;
    Node* CurrentNode = NULL;
    Graph* graph = CreateGraph();

    Vertex* A = CreateVertex('A');
    Vertex* B = CreateVertex('B');
    Vertex* C = CreateVertex('C');
    Vertex* D = CreateVertex('D');
    Vertex* E = CreateVertex('E');
    Vertex* F = CreateVertex('F');
    Vertex* G = CreateVertex('G');
    Vertex* H = CreateVertex('H');

    AddVertex(graph, A);
    AddVertex(graph, B);
    AddVertex(graph, C);
    AddVertex(graph, D);
    AddVertex(graph, E);
    AddVertex(graph, F);
    AddVertex(graph, G);
    AddVertex(graph, H);

    AddEdge(A, CreateEdge(A, C, 0));
    AddEdge(A, CreateEdge(A, D, 0));

    AddEdge(B, CreateEdge(B, C, 0));
    AddEdge(B, CreateEdge(B, E, 0));

    AddEdge(C, CreateEdge(C, F, 0));

    AddEdge(D, CreateEdge(D, F, 0));
    AddEdge(D, CreateEdge(D, G, 0));

    AddEdge(E, CreateEdge(E, G, 0));

    AddEdge(F, CreateEdge(F, H, 0));

    AddEdge(G, CreateEdge(G, H, 0));

    TopologicalSort(graph->Vertices, &SortedList);
    
    fprintf(stdout, "Topological Sort Result : ");

    CurrentNode = SortedList;

    while (CurrentNode != NULL)
    {
        fprintf(stdout, "%C ", CurrentNode->Data->Data);
        CurrentNode = CurrentNode->NextNode;
    }

    fprintf(stdout, "\n");

    DestroyGraph(graph);
    SLL_DestroyAllNodes(&SortedList);

    return 0;
}

int main()
{
    return TS_Test_main();
}