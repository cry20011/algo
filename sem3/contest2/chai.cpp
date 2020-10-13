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

public:

	graph(int n_people, int n_grades, const std::vector<int>& n_grade_bags, const std::vector<std::vector<int>>& lovely_grades) 
	: n_v(n_people + n_grades + 2), start(1), drain(n_people + n_grades + 2), 
	  visited(n_people + n_grades + 2 + 1, 0), 
	  edges((n_people + n_people * n_grades + n_grades) * 2, std::vector<int>(3)),
	  outgoing_edges(n_people + n_grades + 2 + 1, std::vector<int>({}))
	{

		n_e = 0;
		int v_people_first = 2;
		int v_people_last = n_people + 2;
		int v_grade_first	 = v_people_last;
		int v_grade_last = v_grade_first + n_grades;

		for(int i = v_people_first; i < v_people_last; ++i){
		
			edges[n_e * 2][0] = start;
			edges[n_e * 2][1] = i;
			outgoing_edges[start].push_back(n_e * 2);


			edges[n_e * 2 ^ 1][0] = i;
			edges[n_e * 2 ^ 1][1] = start;
			outgoing_edges[i].push_back(n_e * 2 ^ 1);

			++n_e;

			for(int j = 0; j < lovely_grades[i - 2].size(); ++j){
				int grade = lovely_grades[i - 2][j] - 1;

				edges[n_e * 2][0] = i;
				edges[n_e * 2][1] = v_grade_first + grade;
				outgoing_edges[i].push_back(n_e * 2);


				edges[n_e * 2 ^ 1][0] = v_grade_first + grade;
				edges[n_e * 2 ^ 1][1] = i;

				outgoing_edges[v_grade_first + grade].push_back(n_e * 2 ^ 1);

				++n_e;
			}
		}

		for(int i = v_grade_first; i < v_grade_last; ++i){

			edges[n_e * 2][0] = i;
			edges[n_e * 2][1] = drain;
			outgoing_edges[i].push_back(n_e * 2);


			edges[n_e * 2 ^ 1][0] = drain;
			edges[n_e * 2 ^ 1][1] = i;
			outgoing_edges[drain].push_back(n_e * 2 ^ 1);
			++n_e;
		}
		
	}

	void set_cap(int n_people, int n_grades, const std::vector<int>& n_grade_bags, const std::vector<std::vector<int>>& lovely_grades, int m){
		n_e = 0;
		int v_people_first = 2;
		int v_people_last = n_people + 2;
		int v_grade_first	 = v_people_last;
		int v_grade_last = v_grade_first + n_grades;

		for(int i = v_people_first; i < v_people_last; ++i){
			edges[n_e * 2][2] = m;
			edges[n_e * 2 + 1][2] = 0;
			++n_e;

			for(int j = 0; j < lovely_grades[i - 2].size(); ++j){
				int grade = lovely_grades[i - 2][j] - 1;

				edges[n_e * 2][2] = n_grade_bags[grade];
				edges[n_e * 2 + 1][2] = 0;
				++n_e;
			}
		}
		for(int i = v_grade_first; i < v_grade_last; ++i){
			edges[n_e * 2][2] = n_grade_bags[i - v_grade_first];
			edges[n_e * 2 + 1][2] = 0;
			++n_e;
		}

	}




	int find_max_flow(){
		int maxFlow = 0;
		int Flow = 0;

		while((Flow = find_flow(start, INF)) > 0){
			visited.assign(n_v + 1, 0);
			maxFlow += Flow;
		}

		return maxFlow;
	}




private:

	int find_flow(int u, int flow){
		if(u == drain){
			return flow;
		}
		visited[u] = 1;

		for(const auto& e : outgoing_edges[u]){
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



class tea_break{
private:
	int n_people, n_grades;

	std::vector<int> n_grade_bags;
	std::vector<std::vector<int>> lovely_grades;

	bool enough(int m){

		graph g = graph(n_people, n_grades, n_grade_bags, lovely_grades);

		return n_people * m == g.find_max_flow();
	}



public:

	tea_break(int n_people, int n_grades) : n_people(n_people), n_grades(n_grades),
	                                        n_grade_bags(n_grades),
	                                        lovely_grades(n_people)
	{}

	friend std::istream& operator>>(std::istream& in, tea_break& tb);	
	friend class graph;

	int solve(){
		int l = 0;
		int r = 0;
		for(int i = 0; i < n_grades; ++i){
			r += n_grade_bags[i];
		}
		(r /= n_people) += 1;

		graph g = graph(n_people, n_grades, n_grade_bags, lovely_grades);


		while(l + 1 < r){

			int m = (l + r) / 2;
			g.set_cap(n_people, n_grades, n_grade_bags, lovely_grades, m);


			if(n_people * m == g.find_max_flow()){
				l = m;
			}else{
				r = m;
			}
		}
		return l;
	}






};



std::istream& operator>>(std::istream& in, tea_break& tb){
	
	for(int i = 0; i < tb.n_grades; ++i){
		in >> tb.n_grade_bags[i];
	}

	for(int i = 0; i < tb.n_people; ++i){
		int nlovely_grades;
		in >> nlovely_grades;
		tb.lovely_grades[i] = std::vector<int>(nlovely_grades);

		for(int j = 0; j < nlovely_grades; ++j){
			in >> tb.lovely_grades[i][j];
		}
	}

	return in;
}




int main(){
	int n_people, n_grades;
	std::cin >> n_people >> n_grades;

	tea_break tb(n_people, n_grades);

	std::cin >> tb;

	std::cout << tb.solve();
}