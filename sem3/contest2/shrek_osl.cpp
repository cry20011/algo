#include <vector>
#include <iostream>
#include <stack>
#include <limits>
#include <algorithm>
#include <utility>
#include <limits>
#include <functional>
#include <queue>


struct Edge{
    int from;
    int to;
    int cap;
    int flow;
};



class graph{
private:

    const int INF = 1e9;

    std::vector<std::vector<int>> outgoing_edges;
    std::vector<Edge> edges;

    int start;
    int drain;
    int e_n;
    int v_n;

    static bool flow_less_cap(const Edge& e){
        return e.cap > e.flow;
    }

    static bool flow_above_0(const Edge& e){
        return e.flow > 0;
    }

    template<typename Valid_e_func>
    std::vector<int> find_path(Valid_e_func valid){
        std::vector<int> from_v(v_n, -1);

        std::queue<int> q;
        q.push(start);

        while(!q.empty()){
            const int cur = q.front();
            q.pop();

            if(cur == drain)
                break;

            for(const auto& edge_id: outgoing_edges[cur]){
                const auto& Edge = edges[edge_id];

                if(!valid(Edge) || from_v[Edge.to] != -1 || Edge.to == cur)
                    continue;

                from_v[Edge.to] = edge_id;
                q.push(Edge.to);
            }
        }
        return from_v;
    }

    bool push_flow(){
        const auto& from_v = find_path(flow_less_cap);

        if(from_v[drain] == -1)
            return false;

        int min_cap = INF;
        for(int cur = drain; cur != start;){
            int edge_id = from_v[cur];
            int last = edges[edge_id].cap - edges[edge_id].flow;
           
            min_cap = min_cap > last ? last : min_cap;
            cur = edges[edge_id].from;
        }

        for(int cur = drain; cur != start;){
            int edge_id = from_v[cur];
           
            edges[edge_id].flow += min_cap;
            edges[edge_id ^ 1].flow -= min_cap;
            cur = edges[edge_id].from;
        }

        return true;
    }
    
    int summ_flow(){
        int summ_flow = 0;
        for(const auto& edge_id: outgoing_edges[start])
            summ_flow += edges[edge_id].flow;
        return summ_flow;
    }

public:

    graph(int s, int t, int n, int m) : start(s), drain(t), v_n(n), e_n(m), outgoing_edges(n) {
        for(int i = 0, u, v; i < e_n; ++i){
            std::cin >> u >> v;
            add_edge(u - 1, v - 1);
        }
    }

    void add_edge(const int u, const int v){
        outgoing_edges[u].push_back(edges.size());
        edges.push_back({u, v, 1, 0});
        outgoing_edges[v].push_back(edges.size());
        edges.push_back({v, u, 0, 0});
    }


    int find_max_flow(){
        while(push_flow()){}
        return summ_flow();
    }

    std::vector<int> make_path(){
        const auto& from = find_path(flow_above_0);

        if(from[drain] == -1)
            return{};

        std::vector<int> path;

        for(int cur = drain; cur != start;){
            path.push_back(cur);
            int edge_id = from[cur];
            edges[edge_id].flow -= 1;
            cur = edges[edge_id].from;
        }

        path.push_back(start);
        std::reverse(path.begin(), path.end());

        return path;
    }

    friend std::istream& operator>>(std::istream& in, graph& g);
};


int main(){
    int n, m, s, t;
    std::cin >> n >> m >> s >> t;

    graph g(s - 1, t - 1, n, m);
    g.find_max_flow();

    auto shrek_path = g.make_path();
    auto donkey_path = g.make_path();

    if(donkey_path.empty())
        std::cout << "NO";
    else{
        std::cout << "YES\n";
        for(int i = 0; i < shrek_path.size(); ++i){
            std::cout << shrek_path[i] + 1 << ' ';
        }std::cout << '\n';
        for(int i = 0; i < donkey_path.size(); ++i){
            std::cout << donkey_path[i] + 1 << ' ';
        }std::cout << '\n';
    }
}