#include <algorithm>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

class DynamicGraph {
public:
  explicit DynamicGraph(int vertex_count)
      : count_(vertex_count), components_(vertex_count),
        parent_(vertex_count), rank_(vertex_count, 0) {
    for (int index = 0; index < vertex_count; ++index) {
      parent_[index] = index;
    }
  }

  void AddEdge(int verA, int verB) {
    auto edge = std::make_pair(std::min(verA, verB), std::max(verA, verB));
    if (edges_.find(edge) != edges_.end()) {
      return;
    }
    edges_.insert(edge);
    Union(verA, verB);
  }

  void RemoveEdge(int verA, int verB) {
    auto edge = std::make_pair(std::min(verA, verB), std::max(verA, verB));
    if (edges_.find(edge) == edges_.end()) {
      return;
    }
    edges_.erase(edge);
    RecalculateComponents();
  }

  int GetComponentsNumber() const {
    return components_;
  }

private:
  int count_;
  int components_;
  std::set<std::pair<int, int>> edges_;
  std::vector<int> parent_;
  std::vector<int> rank_;

  int Find(int vertex) {
    if (parent_[vertex] != vertex) {
      parent_[vertex] = Find(parent_[vertex]);
    }
    return parent_[vertex];
  }

  void Union(int verA, int verB) {
    int rootA = Find(verA);
    int rootB = Find(verB);

    if (rootA != rootB) {
      if (rank_[rootA] > rank_[rootB]) {
        parent_[rootB] = rootA;
      } else if (rank_[rootA] < rank_[rootB]) {
        parent_[rootA] = rootB;
      } else {
        parent_[rootB] = rootA;
        ++rank_[rootA];
      }
      --components_;
    }
  }

  void RecalculateComponents() {
    components_ = count_;
    for (int index = 0; index < count_; ++index) {
      parent_[index] = index;
      rank_[index] = 0;
    }
    for (const auto &edge : edges_) {
      Union(edge.first, edge.second);
    }
  }
};
