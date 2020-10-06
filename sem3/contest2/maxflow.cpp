#include <iostream>
#include <vector>

const int INF = 1e9;

int n_v, n_e;
std::vector<std::vector<int>> edges;
std::vector<int> visited;

int find_flow(int u, int flow){
	if(u == n_v){
		return flow;
	}
	visited[u] = 1;

	for(int i = 0; i < n_e * 2; ++i){
		int to = edges[i][1];
		int cap = edges[i][2];

		if(edges[i][0] == u && !visited[to] && cap != 0){
			int min_way = find_flow(to, std::min(flow, cap));

			if(min_way > 0){
				edges[i][2] -= min_way;
				edges[i ^ 1][2] += min_way;
				return min_way;
			}
		}
	}
	return 0;
}


bool dfs(int u){
	if(u == n_v){
		return 1;
	}
	visited[u] = 1;

	for(int i = 0; i < n_e * 2; ++i){
		int to = edges[i][1];
		int cap = edges[i][2];

		if(edges[i][0] == u && !visited[to] && cap != 0){
			bool ae = dfs(to);
			if(ae){
				return ae;
			}
		}
	}
	return 0;
}






int main(){

	std::cin >> n_v >> n_e;

	edges = std::vector<std::vector<int>>(n_e * 2, std::vector<int>(3));
	visited = std::vector<int>(n_v + 1, 0);

	for(int i = 0, u, v, cap; i < n_e * 2; i += 2){
		std::cin >> u >> v >> cap;

		edges[i][0] = u;
		edges[i][1] = v;
		edges[i][2] = cap;

		edges[i + 1][0] = v;
		edges[i + 1][1] = u;
		edges[i + 1][2] = 0;

	}

	int maxFlow = 0;
	int Flow = 0;

	while((Flow = find_flow(1, INF)) > 0){
		visited.assign(n_v + 1, 0);
		maxFlow += Flow;
	}
	std::cout << maxFlow;
}