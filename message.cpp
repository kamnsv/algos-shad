#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <tuple>
#include <vector>

struct Node {
    char value;
    int priority;
    int size;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    explicit Node(char val)
        : value(val),
          priority(rand()),
          size(1),
          left(nullptr),
          right(nullptr) {}
};

using NodePtr = std::shared_ptr<Node>;

void Update(NodePtr node) {
    if (node) {
        node->size = 1;
        if (node->left) {
            node->size += node->left->size;
        }
        if (node->right) {
            node->size += node->right->size;
        }
    }
}

void Split(NodePtr root, int key, NodePtr& left, NodePtr& right) {
    if (!root) {
        left = right = nullptr;
    } else if ((root->left ? root->left->size : 0) >= key) {
        Split(root->left, key, left, root->left);
        right = root;
    } else {
        Split(root->right, key - (root->left ? root->left->size : 0) - 1,
              root->right, right);
        left = root;
    }
    Update(root);
}

NodePtr Merge(NodePtr left, NodePtr right) {
    if (!left || !right) {
        return left ? left : right;
    }
    if (left->priority > right->priority) {
        left->right = Merge(left->right, right);
        Update(left);
        return left;
    }
    right->left = Merge(left, right->left);
    Update(right);
    return right;
}

NodePtr Build(const std::string& str) {
    NodePtr root = nullptr;
    for (char ch : str) {
        root = Merge(root, std::make_shared<Node>(ch));
    }
    return root;
}

void Traverse(NodePtr root, std::string& result) {
    if (!root) {
        return;
    }
    Traverse(root->left, result);
    result += root->value;
    Traverse(root->right, result);
}

void ReverseCyclicShift(NodePtr& root, int start, int end, int shift) {
    NodePtr left;
    NodePtr mid;
    NodePtr right;
    Split(root, end, mid, right);
    Split(mid, start - 1, left, mid);

    int length = end - start + 1;
    shift %= length;
    if (shift > 0) {
        NodePtr part_a;
        NodePtr part_b;
        Split(mid, length - shift, part_a, part_b);
        mid = Merge(part_b, part_a);
    }
    root = Merge(Merge(left, mid), right);
}

int main() {
    std::string encrypted;
    std::cin >> encrypted;

    int shifts;
    std::cin >> shifts;

    std::vector<std::tuple<int, int, int>> params(shifts);
    for (int index = 0; index < shifts; ++index) {
        int start;
        int end;
        int shift;
        std::cin >> start >> end >> shift;
        params[index] = std::make_tuple(start, end, shift);
    }
    NodePtr root = Build(encrypted);
    for (int index = shifts - 1; index >= 0; --index) {
        auto [start, end, shift] = params[index];
        ReverseCyclicShift(root, start, end, end - start + 1 - shift);
    }

    std::string decrypted;
    Traverse(root, decrypted);

    std::cout << decrypted << std::endl;
    return 0;
}
