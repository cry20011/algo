#include <iostream>
#include <vector>


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

struct order{
	int time;
	int way_time;
	int x1, y1;
	int x2, y2;
};

int way_time(int x1, int y1, int x2, int y2){
	return abs(x1 - x2) + abs(y1 - y2);
}

int main(){
	int n;
	std::cin >> n;
	std::vector<order> v;
	std::vector<std::vector<int>> e(n, std::vector<int>({}));


	for(int i = 0; i < n; ++i){
		int hours, minutes, x1, y1, x2, y2;
		char c;
		std::cin >> hours >> c >> minutes >> x1 >> y1 >> x2 >> y2;

		int start_time = hours * 60 + minutes;
		v.push_back({start_time, way_time(x1, y1, x2, y2), x1, y1, x2, y2});
	}

	for(int i = 0; i < n; ++i){
		for(int j = i + 1; j < n; ++j){
			if(v[i].time + v[i].way_time + way_time(v[i].x2, v[i].y2, v[j].x1, v[j].y1) < v[j].time)
				e[i].push_back(j);
		}
	}

	bipart_graph g(n, n, e);

	g.find_max_match();

	std::cout << n - g.match_size();

	const int x = n;


}