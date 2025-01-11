#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

class FenwickTree {
public:
    explicit FenwickTree(int size) : tree_(size + 1, 0) {}

    void Add(size_t index, int value) {
        for (++index; index < tree_.size(); index += index & -index) {
            tree_[index] += value;
        }
    }

    int Sum(size_t index) const {
        int result = 0;
        for (++index; index > 0; index -= index & -index) {
            result += tree_[index];
        }
        return result;
    }

    int RangeSum(size_t left, size_t right) const {
        return Sum(right) - Sum(left - 1);
    }

private:
    std::vector<int> tree_;
};

int main() {
    int el;
    std::cin >> el;
    std::vector<int64_t> ar(el);
    for (int index = 0; index < el; ++index) {
        std::cin >> ar[index];
    }
    int64_t lbound;
    int64_t rbound;
    std::cin >> lbound >> rbound;
    std::vector<int64_t> pre(el + 1, 0);
    for (int index = 0; index < el; ++index) {
        pre[index + 1] = pre[index] + ar[index];
    }
    std::vector<int64_t> all = pre;
    for (int64_t sum : pre) {
        all.push_back(sum - lbound);
        all.push_back(sum - rbound - 1);
    }
    std::sort(all.begin(), all.end());
    all.erase(std::unique(all.begin(), all.end()), all.end());
    auto get_index = [&](int64_t value) {
        return std::lower_bound(all.begin(), all.end(), value) - all.begin();
    };
    FenwickTree fenwick(all.size());
    int result = 0;
    for (int64_t prefix : pre) {
        size_t left_index = get_index(prefix - rbound);
        size_t right_index = get_index(prefix - lbound);
        if (left_index <= right_index) {
            result += fenwick.RangeSum(left_index, right_index);
        }
        size_t current_index = get_index(prefix);
        fenwick.Add(current_index, 1);
    }
    std::cout << result << std::endl;
    return 0;
}
