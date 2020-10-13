#include <iostream>
#include <vector>
#include <string>

const int INF = 1e9;

class graph{
private:

	int start;
	int drain;


	std::vector<std::vector<int>> edges;
	std::vector<std::vector<int>> outgoing_edges;
	std::vector<int> visited;
	int n_v, n_e;

public:

	int in_val = 0;
	int out_val = 0;

	friend std::istream& operator>>(std::istream& in, graph& g);

	int find_max_flow(){
		int maxFlow = 0;
		int Flow = 0;

		while((Flow = find_flow(start, INF)) > 0){
			visited.assign(n_v, 0);
			maxFlow += Flow;
		}

		return maxFlow;
	}

	graph(int n, int m, const std::vector<std::string>& paper){

		int n_elems = 0;
		const std::vector<char> elems = {'H', 'O', 'N', 'C'};
		std::vector<std::vector<int>> index(n, std::vector<int>(m, 0));


		for(int i = 0; i < n; ++i){
			for(int j = 0; j < m; ++j){
				if(paper[i][j] != '.'){
					index[i][j] = n_elems + 1;
					++n_elems;
				}
			}
		}

		start = 0;
		drain = n_elems + 1;
		n_v = n_elems + 2;
		n_e = 0;

		visited = std::vector<int>(n_v, 0);
		outgoing_edges = std::vector<std::vector<int>>(n_v, std::vector<int>({}));
			
		for(int i = 0; i < n; ++i){
			for(int j = 0; j < m; ++j){
				bool black = (i + j) % 2;

				char symb = paper[i][j];
				if(symb == '.')
					continue;

				int ind = index[i][j];
				int val = 1;
				for(int k = 0; k < elems.size(); ++k){
					if(elems[k] == symb){
						val += k;
					}
				}

				if(black){
					in_val += val;
					add_edge(start, ind, val);
				}else{
					out_val += val;
					add_edge(ind, drain, val);
				}

				if(!black){
					continue;
				}

				if(i + 1 < n){
					if(paper[i + 1][j] != '.')
						add_edge(ind, index[i + 1][j], 1);
				}
				if(j + 1 < m){
					if(paper[i][j + 1] != '.')
						add_edge(ind, index[i][j + 1], 1);
				}
				if(i - 1 >= 0){
					if(paper[i - 1][j] != '.')
						add_edge(ind, index[i - 1][j], 1);
				}
				if(j - 1 >= 0){
					if(paper[i][j - 1] != '.')
						add_edge(ind, index[i][j - 1], 1);
				}
			}
		}
	}

	void print_edges(){
		std::cout << n_e << '\n';
		for(int i = 0; i < edges.size(); i += 2){
			std::cout << edges[i][0] << ' ' << edges[i][1] << ' ' << edges[i][2] << '\n';

		}
	}


private:

	void add_edge(int v, int u, int cap){
		edges.push_back({v, u, cap});
		edges.push_back({u, v, 0});
		
		outgoing_edges[v].push_back(n_e * 2);
		outgoing_edges[u].push_back(n_e * 2 + 1);
		++n_e;
	}

	int find_flow(int u, int flow){
		if(u == drain){
			return flow;
		}
		visited[u] = 1;

		for(const int& e : outgoing_edges[u]){
			int to = edges[e][1];
			int cap = edges[e][2];

			if(!visited[to] && cap != 0){
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





};


int main(){
	int n, m;
	std::cin >> n >> m;

	std::vector<std::string> paper(n);

	for(auto& l : paper){
		std::cin >> l;
	}

	graph g(n, m, paper);



	if(g.in_val != 0 && g.in_val == g.out_val && g.in_val == g.find_max_flow()){
		std::cout << "Valid";
	}else{
		std::cout << "Invalid";
	}
}
