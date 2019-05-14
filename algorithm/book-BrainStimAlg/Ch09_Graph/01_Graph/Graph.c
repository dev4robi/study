#include "Graph.h"

Graph* CreateGraph()
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->Vertices = NULL;
    graph->VertexCount = 0;
    return graph;
}

void DestroyGraph(Graph* G)
{
    if (G != NULL)
    {
        while (G->Vertices != NULL) // 그래프에 연결된 모든 정점 파괴
        {
            Vertex* Vertices = G->Vertices->Next;
            DestroyVertex(G->Vertices);
            G->Vertices = Vertices;
        }

        free(G);
    }
}

Vertex* CreateVertex(ElementType Data)
{
    Vertex* V = (Vertex*)malloc(sizeof(Vertex));
    V->Data = Data;
    V->Next = NULL;
    V->AdjacencyList = NULL;
    V->Visited = NotVisited;
    V->Index = -1;
    return V;
}

void DestroyVertex(Vertex* V)
{
    if (V != NULL)
    {
        while (V->AdjacencyList != NULL) // 정점에 연결된 모든 간선 파괴
        {
            Edge* edge = V->AdjacencyList->Next;
            DestroyEdge(V->AdjacencyList);
            V->AdjacencyList = edge;
        }

        free(V);
    }
}

Edge* CreateEdge(Vertex* From, Vertex* Target, int Weight)
{
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->From = From;
    edge->Target = Target;
    edge->Next = NULL;
    edge->Weight = Weight;
    return edge;
}

void DestroyEdge(Edge* E)
{
    if (E != NULL) free(E);
}

void AddVertex(Graph* G, Vertex* V)
{
    Vertex* VertexList = G->Vertices;

    if (VertexList == NULL) // 그래프에 정점이 처음 추가됨
    {
        G->Vertices = V;
    }
    else // 그래프에 추가된 정점이 있음
    {
        while (VertexList->Next != NULL)
        {
            VertexList = VertexList->Next;
        }

        VertexList->Next = V;
    }

    V->Index = G->VertexCount++;
}

void AddEdge(Vertex* V, Edge* E)
{
    if (V->AdjacencyList == NULL) // 정점에 간선이 처음 추가됨
    {
        V->AdjacencyList = E;
    }
    else // 정점에 추가된 간선이 있음
    {
        Edge* AdjacencyList = V->AdjacencyList;

        while (AdjacencyList->Next != NULL)
        {
            AdjacencyList = AdjacencyList->Next;
        }

        AdjacencyList->Next = E;
    }
}

void PrintGraph(Graph* G)
{
    Vertex* V = NULL;
    Edge*   E = NULL;

    if (G == NULL || (V = G->Vertices) == NULL) return;

    while (V != NULL)
    {
        fprintf(stdout, "%c : ", V->Data);

        if ( (E = V->AdjacencyList) == NULL)
        {
            V = V->Next;
            fprintf(stdout, "\n");
            continue;
        }

        while (E != NULL)
        {
            fprintf(stdout, "%c[%d] ", E->Target->Data, E->Weight);
            E = E->Next;
        }

        fprintf(stdout, "\n");
        V = V->Next;
    }

    fprintf(stdout, "\n");
}

static int Graph_Test_main()
{
    Graph*  G  = CreateGraph();
    Vertex* V1 = CreateVertex('1');
    Vertex* V2 = CreateVertex('2');
    Vertex* V3 = CreateVertex('3');
    Vertex* V4 = CreateVertex('4');
    Vertex* V5 = CreateVertex('5');

    AddVertex(G, V1);
    AddVertex(G, V2);
    AddVertex(G, V3);
    AddVertex(G, V4);
    AddVertex(G, V5);

    AddEdge(V1, CreateEdge(V1, V2, 0));
    AddEdge(V1, CreateEdge(V1, V3, 0));
    AddEdge(V1, CreateEdge(V1, V4, 0));
    AddEdge(V1, CreateEdge(V1, V5, 0));

    AddEdge(V2, CreateEdge(V2, V1, 0));
    AddEdge(V2, CreateEdge(V2, V3, 0));
    AddEdge(V2, CreateEdge(V2, V5, 0));

    AddEdge(V3, CreateEdge(V3, V1, 0));
    AddEdge(V3, CreateEdge(V3, V2, 0));

    AddEdge(V4, CreateEdge(V4, V1, 0));
    AddEdge(V4, CreateEdge(V4, V5, 0));

    AddEdge(V5, CreateEdge(V5, V1, 0));
    AddEdge(V5, CreateEdge(V5, V2, 0));
    AddEdge(V5, CreateEdge(V5, V4, 0));

    PrintGraph(G);
    DestroyGraph(G);

    return 0;
}

int main()
{
    return Graph_Test_main();
}