#include <iostream>
#include <vector>

class FenwickTree {
public:
    explicit FenwickTree(size_t size) : tree_(size + 1, 0) {}

    void Update(size_t index, size_t value) {
        for (; index < tree_.size(); index += index & -index) {
            tree_[index] += value;
        }
    }

    size_t Query(size_t index) {
        size_t sum = 0;
        for (; index > 0; index -= index & -index) {
            sum += tree_[index];
        }
        return sum;
    }

    size_t FindKth(size_t kth) {
        size_t index = 0;
        size_t mask = 1;
        while (mask < tree_.size()) {
            mask <<= 1;
        }
        for (; mask > 0; mask >>= 1) {
            size_t next_index = index + mask;
            if (next_index < tree_.size() && tree_[next_index] < kth) {
                kth -= tree_[next_index];
                index = next_index;
            }
        }
        return index + 1;
    }

private:
    std::vector<size_t> tree_;
};

int main() {
    size_t count;
    size_t kik;
    std::cin >> count >> kik;

    FenwickTree fenwick(count);
    for (size_t index = 1; index <= count; ++index) {
        fenwick.Update(index, 1);
    }

    std::vector<size_t> elimination_order;
    size_t current_pos = 0;

    for (size_t pos = 0; pos < count; ++pos) {
        size_t remaining = count - pos;
        current_pos = (current_pos + kik - 1) % remaining;
        size_t index = fenwick.FindKth(current_pos + 1);
        elimination_order.push_back(index);
        fenwick.Update(index, -1);
    }

    for (size_t num : elimination_order) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
