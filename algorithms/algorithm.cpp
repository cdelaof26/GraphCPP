#include "algorithm.h"

void print_shortest_path(int n, int array[n],int source){
    for(int i = 0; i < n; i++){
        printf("%d  -> %d  cost: %d\n", source, i, array[i]);
    }
}

int get_matrix_length(const std::string& filename){
    std::ifstream file(filename);
    if (!file.is_open())
        return 0;

    std::string s;
    std::getline(file, s);
    std::getline(file, s);
    file.close();

    int count = 1;
    for (char ch : s)
        if (ch == ' ')
            count++;

    return count;
}

void create_matrix_from_file(const std::string& filename, int n, std::string& algorithmName, std::vector<std::vector<int>>& matrix) {
    if (n == 0)
        return;

    std::ifstream file(filename);
    if (!file.is_open())
        return;

    std::getline(file, algorithmName);

    for (int i = 0; i < n; i++)
        for (int j = 0 ; j < n; j++) {
            file >> matrix[i][j];
            if (matrix[i][j] < 0)
                matrix[i][j] = INF;
        }

    file.close();
}

void create_matrix(int n, std::vector<std::vector<int>>& matrix, int value) {
	for (int i = 0; i < n; i++)
		for(int j = 0 ; j < n; j++)
            matrix[i][j] = value;
}

void insert_edge(int n, std::vector<std::vector<int>>& matrix, int u, int v, int w){
	matrix [u][v] = matrix [v][u] = w;
}

void print_matrix(int n, std::vector<std::vector<int>>& matrix){
	for (int i = 0; i < n; i++){
		for(int j = 0 ; j < n; j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

std::vector<std::vector<std::vector<int>>> * kruskal(int n, std::vector<std::vector<int>>& adjacency_matrix, std::vector<std::vector<int>>& mst) {
	create_matrix(n, mst, 0);
    int belong_nodes[n];

	for(int i = 0; i < n; i++){
		belong_nodes[i] = i;
	}

	//reference nodes
	int nodeA;
	int nodeB;
	//the total of edges is equal to total_nodes - 1;
	int edges = 0;

    auto * states = new std::vector<std::vector<std::vector<int>>>();

	while (edges < n - 1){
        printf("total edges; %d \n", edges);
		int min = INF;
		for (int i = 0; i < n ; i++ ){
			for (int j = i + 1; j < n; j++ ){
				if (min > adjacency_matrix[i][j] && adjacency_matrix[i][j] && belong_nodes[i] != belong_nodes[j]){
					min = adjacency_matrix[i][j];
					nodeA = i;
					nodeB = j;
				}
			}
		}
		// add to MST if the nodes don't belong same tree
		if (belong_nodes[nodeA] != belong_nodes[nodeB]) {
			insert_edge(n, mst, nodeA, nodeB, min);
			states -> push_back(mst);
            print_matrix(n, mst);
			//add all nodes from nodeB to nodeA
			int temp = belong_nodes[nodeB];
			belong_nodes[nodeB] = belong_nodes[nodeA];
			for (int i = 0 ; i < n; i++) if (temp == belong_nodes[i]) belong_nodes[i] = belong_nodes[nodeA];
			edges++;
		}
	}

    return states;
}

std::vector<std::vector<std::vector<int>>> * prim(int n, std::vector<std::vector<int>>& adjacency_matrix, std::vector<std::vector<int>>& mst){
	create_matrix(n, mst, 0);
	int belong_nodes[n];
	belong_nodes[0] = 1;
	for(int i = 1; i < n; i++){
		belong_nodes[i] = 0;
	}

	//reference nodes
	int nodeA;
	int nodeB;
	//the total of edges is equal to total_nodes - 1
	int edges = 0;

    auto * states = new std::vector<std::vector<std::vector<int>>>();

	while (edges < n - 1){
		printf("total edges; %d \n", edges);
		int min = INF;
		for (int i = 0; i < n ; i++ ){
			if(belong_nodes[i]){
				for (int j = 0; j < n; j++ ){
					if (min > adjacency_matrix[i][j] && adjacency_matrix[i][j] && !belong_nodes[j]){
						min = adjacency_matrix[i][j];
						nodeA = i;
						nodeB = j;
					}
				}
			}
		}
		// add new node to mst
		insert_edge(n, mst, nodeA, nodeB, min);
        states -> push_back(mst);
		print_matrix(n, mst);
		//check the node added
		belong_nodes[nodeB] = 1;
		edges++;
	}

    return states;
}

void dijkstra(int n, int source, std::vector<std::vector<int>>& matrix, int route[n]){
	int flag[n];
	int min, pos_min;
	for (int i = 0; i < n; i++){
		flag[i] = 0;
		route[i] = matrix[source][i];
	}

	route[source] = 0;

	for(int i = 1; i < n; i++){
		min = INF;
		for(int j = 0; j < n; j++){
			if((route[j] < min) && !flag[j]){
				min = route[j];
				pos_min = j;
			}
		}

		flag[pos_min] = 1;
		for(int j = 0; j < n; j++){
			if(((route[pos_min] + matrix[pos_min][j]) < route[j]) && flag[j] != 1)
				route[j] = route[pos_min] + matrix[pos_min][j];
		}
	}
}
