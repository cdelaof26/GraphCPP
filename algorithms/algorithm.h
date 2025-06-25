#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <cstdio>
#include <vector>
#include <fstream>

#define MAX_NODES 100
#define INF 100000

void print_shortest_path(int n, int array[n],int source);

int get_matrix_length(const std::string& filename);

void create_matrix_from_file(const std::string& filename, int n, std::string& algorithmName, std::vector<std::vector<int>>& matrix);

void create_matrix(int n, std::vector<std::vector<int>>& matrix, int value);

void insert_edge(int n, std::vector<std::vector<int>>& matrix, int u, int v, int w);

void print_matrix(int n, std::vector<std::vector<int>>& matrix);

std::vector<std::vector<std::vector<int>>> * kruskal(int n, std::vector<std::vector<int>>& adjacency_matrix, std::vector<std::vector<int>>& mst);

std::vector<std::vector<std::vector<int>>> * prim(int n, std::vector<std::vector<int>>& adjacency_matrix, std::vector<std::vector<int>>& mst);

void dijkstra(int n, int source, std::vector<std::vector<int>>& matrix, int route[n]);

#endif
