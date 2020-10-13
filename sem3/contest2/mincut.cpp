#include <iostream>
#include <vector>

const int INF = 1e9;

class graph{
private:

	int start;
	int drain;

	std::vector<std::vector<int>> edges;
	std::vector<std::vector<int>> outgoing_edges;
	std::vector<int> visited;
	int n_v, n_e;

	int g_maxFlow;

public:

	friend std::istream& operator>>(std::istream& in, graph& g);

	graph(int n_v, int n_e, int s, int t) : n_v(n_v), n_e(n_e), start(s), drain(t), 
	                          edges(n_e * 4, std::vector<int>(3)), 
	                          visited(n_v + 1, 0),
	                          outgoing_edges(n_v + 1, std::vector<int>({}))
	{}

	int find_max_flow(){
		int maxFlow = 0;
		int Flow = 0;

		while((Flow = find_flow(start, INF)) > 0){
			visited.assign(n_v + 1, 0);
			maxFlow += Flow;
		}

		g_maxFlow = maxFlow;

		return maxFlow;
	}



	int find_min_cut(){
		int n_edges_in_cut = 0;
		visited.assign(n_v + 1, 0);
		
		dfs(start);
		for(int i = 0; i < n_e * 4; i += 2){
			if(visited[edges[i][0]] && !visited[edges[i][1]])
				++n_edges_in_cut;
		}

		std::cout << n_edges_in_cut << ' ' << g_maxFlow << '\n';


		for(int i = 0; i < n_e * 4; i += 2){
			if(visited[edges[i][0]] && !visited[edges[i][1]])
				std::cout << i / 4 + 1 << ' ';
		}

	}



private:

	int find_flow(int u, int flow){
		if(u == drain){
			return flow;
		}
		visited[u] = 1;

		for(const int& e : outgoing_edges[u]){
			int to = edges[e][1];
				int cap = edges[e][2];

			if(edges[e][0] == u && !visited[to] && cap != 0){
				int min_way = find_flow(to, std::min(flow, cap));

				if(min_way > 0){
					edges[e][2] -= min_way;
					edges[e ^ 1][2] += min_way;
					return min_way;
				}
			}
		}
		return 0;
	}


	bool dfs(int u){
		if(u == drain){
			return 1;
		}
		visited[u] = 1;

		for(const int& e : outgoing_edges[u]){
			int to = edges[e][1];
			int cap = edges[e][2];

			if(!visited[to] && cap != 0){
				bool ae = dfs(to);
				if(ae){
					return ae;
				}
			}
		}
		return 0;
	}




};


std::istream& operator>>(std::istream& in, graph& g){

	for(int i = 0, u, v, cap; i < g.n_e * 4; i += 4){
		std::cin >> u >> v >> cap;

		g.edges[i][0] = u;
		g.edges[i][1] = v;
		g.edges[i][2] = cap;
		g.outgoing_edges[u].push_back(i);

		g.edges[i + 1][0] = v;
		g.edges[i + 1][1] = u;
		g.edges[i + 1][2] = 0;
		g.outgoing_edges[v].push_back(i + 1);

		g.edges[i + 2][0] = v;
		g.edges[i + 2][1] = u;
		g.edges[i + 2][2] = cap;
		g.outgoing_edges[v].push_back(i + 2);

		g.edges[i + 3][0] = u;
		g.edges[i + 3][1] = v;
		g.edges[i + 3][2] = 0;
		g.outgoing_edges[u].push_back(i + 3);

	}

	return in;
}



int main(){
	int n_v, n_e;
	std::cin >> n_v >> n_e;

	graph g(n_v, n_e, 1, n_v);

	std::cin >> g;
	g.find_max_flow();
	g.find_min_cut();
}