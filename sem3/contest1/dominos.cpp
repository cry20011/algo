#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int n_free_cells = 0;
int n, m;

class bipart_graph{
private:

	int A_size;
	int B_size;
	std::vector<std::vector<int>> edges;
	std::vector<int> curr_match;
	std::vector<int> been_vert;
	

	bool koon(int v){
		if(been_vert[v] == true)
			return false;
		been_vert[v] = true;

		for(int i = 0; i < edges[v].size(); ++i){
			int next = edges[v][i];
			if(curr_match[next] == -1 || koon(curr_match[next])){
				curr_match[next] = v;
				return true;
			}
		}
		return false;
	}

public:
	bipart_graph(int A, int B, std::vector<std::vector<int>>& e): A_size(A), B_size(B), edges(e){
		
		curr_match.assign(B_size, -1);
		been_vert.assign(A_size, 0);
	}

	void find_max_match(){
		for(int v = 0; v < A_size; ++v){
			been_vert.assign(A_size, 0);
			koon(v);
		}
	}

	int match_size(){
		int size = 0;
		for(int i = 0; i < B_size; ++i)
			if(curr_match[i] != -1)
				size++;

		return size;
	}
};


void White_Black(std::vector<std::string> field, std::vector<std::pair<int, int>>& White, std::vector<std::pair<int, int>>& Black){

	for(int i = 0; i < n; ++i){
		for(int j = 0; j < m; ++j){
			if((i % 2 == 0 && j % 2 == 0 || i % 2 == 1 && j % 2 == 1) && field[i][j] == '*')
				White.push_back({i, j});
			else if(field[i][j] == '*')
				Black.push_back({i, j});
		}

	}

}


int find_black_numb(int y, int x, std::vector<std::pair<int, int>> Black){
	for(int i = 0; i < Black.size(); ++i){
		if(Black[i].first == y && Black[i].second == x)
			return i;
	}
}

void make_edges(std::vector<std::string> field, std::vector<std::vector<int>>& e, std::vector<std::pair<int, int>> White, std::vector<std::pair<int, int>> Black){

	for(int i = 0; i < White.size(); ++i){
		int y = White[i].first;
		int x = White[i].second;

		if(x > 0 && field[y][x - 1] == '*')
			e[i].push_back(find_black_numb(y, x - 1, Black));
		
		if(x < m - 1 && field[y][x + 1] == '*')
			e[i].push_back(find_black_numb(y, x + 1, Black));
		
		if(y > 0 && field[y - 1][x] == '*')
			e[i].push_back(find_black_numb(y - 1, x, Black));
		
		if(y < n - 1 && field[y + 1][x] == '*')
			e[i].push_back(find_black_numb(y + 1, x, Black));
	}
}


int main(){

	std::ifstream fin("dominoes.in");
	std::ofstream fout("dominoes.out");

	int a, b;
	fin >> n >> m >> a >> b;
	
	std::vector<std::string> field(n);
	std::vector<std::pair<int, int>> White;
	std::vector<std::pair<int, int>> Black;



	for(int i = 0; i < n; ++i){
		fin >> field[i];
	}

	White_Black(field, White, Black);
	n_free_cells = White.size() + Black.size();
	if(a > b * 2){
		fout << b * n_free_cells;
		
	}else{
		std::vector<std::vector<int>> e(White.size(), std::vector<int>({}));

		make_edges(field, e, White, Black);

		bipart_graph g = bipart_graph(White.size(), Black.size(), e);
		g.find_max_match();
		int match_size = g.match_size();
		
		fout << match_size * a + (n_free_cells - match_size * 2) * b;
	}

}


