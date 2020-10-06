#include<iostream>
#include<vector>

class bipart_graph{
private:
	int A_size;
	int B_size;
	std::vector<std::vector<int>> edges;
	std::vector<int> curr_match;
	std::vector<int> been_vert;

	void insert_edges(int n){
		edges.push_back({});
		for(int i = 1; i <= n; ++i){
			edges.push_back({});
			int b_vert;
			std::cin >> b_vert;
			while(b_vert != 0){
				edges[i].push_back(b_vert);
				std::cin >> b_vert;
			}
		}
	}

	bool koon(int v){
		if(been_vert[v] == true)
			return false;
		been_vert[v] = true;

		for(int i = 0; i < edges[v].size(); ++i){
			int next = edges[v][i];
			if(curr_match[next] == 0 || koon(curr_match[next])){
				curr_match[next] = v;
				return true;
			}
		}
		return false;
	}


public:
	bipart_graph(int A, int B): A_size(A), B_size(B){
		insert_edges(A_size);
		curr_match.assign(B_size + 1, 0);
		been_vert.assign(A_size + 1, 0);
	}


	void print_edges(){
		for(int i = 1; i <= A_size; ++i){
			std::cout << i << ':';
			for(int j = 0; j < edges[i].size(); ++j){
				std::cout << edges[i][j] << ' ';
			}
			std::cout << '\n';
		}
	}



	void find_max_match(){
		for(int v = 1; v <= A_size; ++v){
			been_vert.assign(A_size + 1, 0);
			koon(v);
		}
	}

	void print_max_match(){
		int size = 0;
		for(int i = 1; i <= B_size; ++i)
			if(curr_match[i] != 0)
				size++;
		std::cout << size << '\n';

		for(int i = 1; i <= B_size; ++i){
			if(curr_match[i] != 0)
				std::cout << curr_match[i] << ' ' << i << '\n';
		}
	}


};



int main(){
	int n, m;
	std::cin >> n >> m;
	bipart_graph g = bipart_graph(n, m); 
//	g.print_edges();
	g.find_max_match();
	g.print_max_match();


}