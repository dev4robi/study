#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include "Graph.h"
#include "PriorityQueue.h"

#define MAX_WEIGHT 36267

// 다익스트라 알고리즘
void Dijkstra(Graph* G, Vertex* StartVertex, Graph* MST);

#endif