#include <iostream>
#include <vector>

class CarnotaurusGuardians {
public:
    explicit CarnotaurusGuardians() {
        parent_.push_back(0);
        parent_.push_back(1);
    }

    void AddDinosaur(size_t guardian) { parent_.push_back(guardian); }

    void RemoveDinosaur(size_t guardian) {
        size_t parent = parent_[guardian];
        for (size_t index = 1; index < parent_.size(); ++index) {
            if (parent_[index] == guardian) {
                parent_[index] = parent;
            }
        }
        parent_[guardian] = parent;
    }

    int FindCommonGuardian(size_t dino_u, size_t dino_v) {
        if (dino_v == dino_u) {
            return dino_u;
        }
        if (parent_[dino_v] == dino_u) {
            return dino_u;
        }
        if (parent_[dino_u] == dino_v) {
            return dino_v;
        }
        size_t root_u = parent_[dino_u];
        size_t root_v = parent_[dino_v];
        while (root_u != root_v) {
            if (root_u == dino_v) {
                return root_u;
            }
            if (root_v == dino_u) {
                return root_v;
            }
            if (root_u > root_v) {
                root_u = parent_[root_u];
            } else {
                root_v = parent_[root_v];
            }
        }
        return root_u;
    }

private:
    std::vector<size_t> parent_;
};

int main() {
    CarnotaurusGuardians guardians;
    char action;
    size_t dino_a;
    size_t dino_b;
    size_t count_actions;
    std::cin >> count_actions;

    for (size_t step = 0; step < count_actions; ++step) {
        std::cin >> action;
        if ('+' == action) {
            std::cin >> dino_a;
            guardians.AddDinosaur(dino_a);
        } else if ('-' == action) {
            std::cin >> dino_a;
            guardians.RemoveDinosaur(dino_a);
        } else if ('?' == action) {
            std::cin >> dino_a >> dino_b;
            std::cout << guardians.FindCommonGuardian(dino_a, dino_b) << '\n';
        }
    }

    return 0;
}
