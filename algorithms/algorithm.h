#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <cstdio>
#include <vector>

#define MAX_NODES 100
#define INF 100000

void print_shortest_path(int n, int array[n],int source);

void create_matrix(int n, std::vector<std::vector<int>>& matrix, int value);

void insert_edge(int n, std::vector<std::vector<int>>& matrix, int u, int v, int w);

void print_matrix(int n, std::vector<std::vector<int>>& matrix);

void kruskal(int n, std::vector<std::vector<int>>& adjacency_matrix, std::vector<std::vector<int>>& mst);

void prim(int n, std::vector<std::vector<int>>& adjacency_matrix, std::vector<std::vector<int>>& mst);

void dijkstra(int n, int source, std::vector<std::vector<int>>& matrix, int route[n]);

#endif
