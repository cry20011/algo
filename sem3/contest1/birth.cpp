#include <iostream>
#include <vector>
#include <queue>

using vi = std::vector<int>;
using vc = std::vector<char>;


bool koon_alg(int v, const std::vector<vi>& graph, vi& match, vc& been){
	if(been[v])
		return false;
	been[v] = true;

	for(const auto& right_v : graph[v]){
		if(match[right_v] == -1 || koon_alg(match[right_v], graph, match, been)) {
			match[right_v] = v;

			return true;
		}
	}
	return false;
}

int main(){
	int K = 0;

	int n_boys = 0;
	int n_girls = 0;

	std::cin >> K;

	std::vector<vi> friend_g;
	std::vector<vi> not_friend_g;

	vi match;
	vc been;

	for(int i = 0; i < K; ++i){
		friend_g.clear();
		not_friend_g.clear();

		std::cin >> n_boys >> n_girls;

		friend_g.resize(n_boys);
		not_friend_g.resize(n_boys);
		match.assign(n_girls, -1);

		for(int boy = 0; boy < n_boys; ++boy){
			int girl = 1;
			while (girl != -1) {
				std::cin >> girl;
				--girl;
				if (girl != -1)
				friend_g[boy].push_back(girl);
			}
		}
		vc friend_(n_girls, 0);

		for(int boy = 0; boy < n_boys; ++boy){
			for (int girl : friend_g[boy])
			friend_[girl] = 1;

			for (int girl = 0; girl < n_girls; ++girl)
				if (!friend_[girl])
					not_friend_g[boy].push_back(girl);

			friend_.assign(n_girls, 0);
		}

		been.assign(n_boys, 0);

		for(int boy = 0; boy < n_boys; ++boy){
			if(koon_alg(boy, not_friend_g, match, been))
				been.assign(n_boys, 0);
		}

		vc nofriend(n_boys, 0);

		for(int girl = 0; girl < n_girls; ++girl)
			if(match[girl] != -1)
				nofriend[match[girl]] = true;

		vc boys_inv(n_boys, 0);
		vc girls_inv(n_girls, 0);

		std::queue<int> boys_ch;
		std::queue<int> girls_ch;

		for(int boy = 0; boy < n_boys; ++boy){
			if(!nofriend[boy]){
				boys_inv[boy] = true;
				for(int girl : not_friend_g[boy])
					girls_ch.push(girl);
			}
		}

		while(!boys_ch.empty() || !girls_ch.empty()){
			while(!boys_ch.empty()){
				int boy = boys_ch.front();
				boys_inv[boy] = true;
				boys_ch.pop();

				for(int girl : not_friend_g[boy])
					if(!girls_inv[girl])
						girls_ch.push(girl);
			}

			while(!girls_ch.empty()){
				int girl = girls_ch.front();
				girls_inv[girl] = true;
				girls_ch.pop();

				if(match[girl] != -1 && !boys_inv[match[girl]])
				boys_ch.push(match[girl]);
			}
		}

		int b_inv_c = 0;

		for(int boy = 0; boy < n_boys; ++boy)
			if(boys_inv[boy])
				++b_inv_c;

		int g_inv_c = 0;

		for(int girl = 0; girl < n_girls; ++girl)
			if(!girls_inv[girl])
				++g_inv_c;

		std::cout << b_inv_c << ' ' << g_inv_c << '\n';

		for(int boy = 0; boy < n_boys; ++boy)
			if(boys_inv[boy])
				std::cout << boy + 1 << ' ';

		std::cout << '\n';

		for(int girl = 0; girl < n_girls; ++girl)
			if(!girls_inv[girl])
				std::cout << girl + 1 << ' ';

		std::cout << '\n' << '\n';
	}

	return 0;
}
