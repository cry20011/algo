#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>


class graph{
private:
	
	struct Edge{
		int from;
		int to;
		int cap;
		int cost;
		long long flow;
	};

	std::vector<Edge> edges;
    std::vector<std::vector<int>> outgoing_edges;

	int start;
	int drain;
	int n_v;
	int n_e;

	const int INF = 1e9;

	static bool flow_less_cap(const Edge& e){
		return e.cap > e.flow;
	}

	static bool flow_above_0(const Edge& e){
		return e.flow > 0;
	}

	template<typename Valid_e_func>
	std::vector<int> find_path(Valid_e_func valid){
		std::vector<int> from_v(n_v, -1);

		std::queue<int> q;
		q.push(start);

		while(!q.empty()){
			int cur_v = q.front();
			q.pop();

			if(cur_v == drain)
				break;

			for(auto& e : outgoing_edges[cur_v]){
				Edge edge = edges[e];

				if(!valid(edge) || from_v[edge.to] != -1 || edge.to == cur_v)
					continue;

				from_v[edge.to] = e;
				q.push(edge.to);
			}
		}

		return from_v;
	}


	bool push_flow(){
		std::vector<int> from_v = find_path(flow_less_cap);

		if(from_v[drain] == -1)
			return false;

		int min_cap = INF;

		for(int cur_v = drain; cur_v != start;){
			int e = from_v[cur_v];
			int last = edges[e].cap - edges[e].flow;

			min_cap = min_cap > last ? last : min_cap;
			cur_v = edges[e].from;
		}

		for(int cur_v = drain; cur_v != start;){
			int e = from_v[cur_v];

			edges[e].flow += min_cap;
			edges[e ^ 1].flow -= min_cap;
			cur_v = edges[e].from;
		}

		return true;
	}

	long long summ_flow(){
		long long summ_flow = 0;
		for(auto& e : outgoing_edges[start]){
			summ_flow += edges[e].flow;
		}
		return summ_flow;
	}
public:


	std::vector<int> find_neg_cycle() {
		std::vector<int> dist(n_v, INF);
		std::vector<int> prev_edge(n_v, -1);
		for(int k = 0; k < n_v; ++k){

			int x;
			dist[k] = 0;
			for(int i = 0; i < n_v; ++i){
				x = -1;
				for(int e = 0; e < n_e * 2; ++e)
					if(flow_less_cap(edges[e]) && dist[edges[e].from] < INF)
						if(dist[edges[e].to] > dist[edges[e].from] + edges[e].cost){
							dist[edges[e].to] = std::max(-INF, dist[edges[e].from] + edges[e].cost);
							prev_edge[edges[e].to] = e;
							x = edges[e].to;
						}
			}
		 
			if(x != -1){
				int y = x;
				for(int i = 0; i < n_v; ++i)
					y = edges[prev_edge[y]].from;
		 
				std::vector<int> path;
				for(int cur = y; ; cur = edges[prev_edge[cur]].from){
					path.push_back(prev_edge[cur]);
					if(cur == y && path.size() > 1)
						break;
				}
				std::reverse(path.begin(), path.end());
		 		return path;
			}
		}
		return {};
	}


	void push_max_cycle_flow(const std::vector<int>& path){
		long long max_possible_flow = INF;
		for(int i = 0; i < path.size() - 1; ++i){
			long long possible_flow = edges[path[i]].cap - edges[path[i]].flow;
			if(possible_flow < max_possible_flow)
				max_possible_flow = possible_flow;
		}
		for(int i = 0; i < path.size() - 1; ++i){
			edges[path[i]].flow += max_possible_flow;
			edges[path[i] ^ 1].flow -= max_possible_flow;
		}
	}







public:

	graph(int v, int e, int s, int t) : n_v(v), n_e(e), start(s), drain(t),
	                                    edges(0), outgoing_edges(v, std::vector<int>({}))
	{
		for(int i = 0, u, v, cap, cost; i < n_e; ++i){
			std::cin >> u >> v >> cap >> cost;
			add_edge(u - 1, v - 1, cap, cost);
		}
	}

	long long find_min_cost_max_flow(){
		find_max_flow();
		std::vector<int> path;
		while(!(path = find_neg_cycle()).empty()){
			push_max_cycle_flow(path);			
		}
		long long summ_costs = 0;
		for(const auto& edge : edges){
			if(edge.flow > 0)
				summ_costs += edge.flow * edge.cost;
		}	
		return summ_costs;	
	}



	void add_edge(int u, int v, int cap, int cost){
		outgoing_edges[u].push_back(edges.size());
		edges.push_back({u, v, cap, cost, 0});
		outgoing_edges[v].push_back(edges.size());
		edges.push_back({v, u, 0, -cost, 0});
	}

	void print_edges(){
		std::cout << n_e << '\n'; 
		for(int i = 0; i < n_e * 2; ++i){
//			if(flow_less_cap(edges[i]))
				std::cout << i << " : " << edges[i].from << ' ' << edges[i].to << ' ' <<edges[i].cap << ' ' << edges[i].cost << ' ' << edges[i].flow << '\n';
		}

		for(int i = 0; i < n_v; ++i){
			std::cout << i << " : ";
			for(int j = 0; j < outgoing_edges[i].size(); ++j){
				std::cout << outgoing_edges[i][j] << ' ';
			}std::cout << '\n';
		}
	}



	int find_max_flow(){
		while(push_flow()){}

		return summ_flow();
	}






};









int main(){
	int n, m;
	std::cin >> n >> m;
	graph g(n, m, 0, n - 1);
	std::cout << g.find_min_cost_max_flow();

//	g.print_edges();
	
/*	if(cycle.empty()){
		std::cout << "(((\n";
	}else{
		std::cout << ")))\n";
		for(int i = 0; i < cycle.size(); ++i)
			std::cout << cycle[i] << ' ';
	}
*/

}