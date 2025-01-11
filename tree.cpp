#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
    int start;
    int end;
    int weight;
};

class WeightedGraph {
public:
    explicit WeightedGraph(int count) {
        parent_.resize(count);
        rank_.resize(count);
        for (int index = 0; index < count; ++index) {
            parent_[index] = index;
            rank_[index] = 1;
        }
    }

    int Find(int vertex) {
        if (parent_[vertex] != vertex) {
            parent_[vertex] = Find(parent_[vertex]);
        }
        return parent_[vertex];
    }

    void Union(int ver_a, int ver_b) {
        int root_a = Find(ver_a);
        int root_b = Find(ver_b);

        if (root_a == root_b) {
            return;
        }

        if (rank_[root_a] > rank_[root_b]) {
            parent_[root_b] = root_a;
        } else if (rank_[root_a] < rank_[root_b]) {
            parent_[root_a] = root_b;
        } else {
            parent_[root_b] = root_a;
            ++rank_[root_a];
        }
    }

private:
    std::vector<int> parent_;
    std::vector<int> rank_;
};

int main() {
    int count_vertex;
    int count_edges;
    std::cin >> count_vertex >> count_edges;

    std::vector<Edge> edges(count_edges);
    for (auto& edge : edges) {
        std::cin >> edge.start >> edge.end >> edge.weight;
        --edge.start;
        --edge.end;
    }

    sort(edges.begin(), edges.end(),
         [](const Edge& ef, const Edge& es) { return ef.weight < es.weight; });

    WeightedGraph graph(count_vertex);
    int max_edge_weight = 0;
    for (auto& edge : edges) {
        if (graph.Find(edge.start) != graph.Find(edge.end)) {
            graph.Union(edge.start, edge.end);
            max_edge_weight = edge.weight;
            if (--count_vertex == 1) {
                break;
            }
        }
    }

    std::cout << max_edge_weight;
    return 0;
}
