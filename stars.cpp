#include <iostream>
#include <vector>

const size_t kCountCoordsCube = 6;

class FenwickSpace {
public:
    explicit FenwickSpace(size_t size)
        : size_(size),
          tree_(size, std::vector<std::vector<int>>(
                          size, std::vector<int>(size, 0))) {}

    void Update(size_t x_coord, size_t y_coord, size_t z_coord, int delta) {
        for (size_t xi = x_coord; xi < size_; xi = xi | (xi + 1)) {
            for (size_t yi = y_coord; yi < size_; yi = yi | (yi + 1)) {
                for (size_t zi = z_coord; zi < size_; zi = zi | (zi + 1)) {
                    tree_[xi][yi][zi] += delta;
                }
            }
        }
    }

    uint Query(int x_coord, int y_coord, int z_coord) const {
        uint sum = 0;
        for (int xi = x_coord; xi >= 0; xi = (xi & (xi + 1)) - 1) {
            for (int yi = y_coord; yi >= 0; yi = (yi & (yi + 1)) - 1) {
                for (int zi = z_coord; zi >= 0; zi = (zi & (zi + 1)) - 1) {
                    sum += tree_[xi][yi][zi];
                }
            }
        }
        return sum;
    }

    uint QueryRange(const std::vector<int> kCube) const {
        int xi = kCube[0];
        int yi = kCube[1];
        int zi = kCube[2];
        int xj = kCube[3];
        int yj = kCube[4];
        int zj = kCube[5];
        return Query(xj, yj, zj) - (xi > 0 ? Query(xi - 1, yj, zj) : 0) -
               (yi > 0 ? Query(xj, yi - 1, zj) : 0) -
               (zi > 0 ? Query(xj, yj, zi - 1) : 0) +
               (xi > 0 && yi > 0 ? Query(xi - 1, yi - 1, zj) : 0) +
               (xi > 0 && zi > 0 ? Query(xi - 1, yj, zi - 1) : 0) +
               (yi > 0 && zi > 0 ? Query(xj, yi - 1, zi - 1) : 0) -
               (xi > 0 && yi > 0 && zi > 0 ? Query(xi - 1, yi - 1, zi - 1) : 0);
    }

private:
    size_t size_;
    std::vector<std::vector<std::vector<int>>> tree_;
};

int main() {
    size_t size;
    std::cin >> size;
    FenwickSpace space(size);
    bool should = true;
    while (should) {
        int code;
        std::cin >> code;
        if (1 == code) {
            std::vector<size_t> coords(3);
            int delta;
            std::cin >> coords[0] >> coords[1] >> coords[2] >> delta;
            space.Update(coords[0], coords[1], coords[2], delta);
        } else if (2 == code) {
            std::vector<int> cube(kCountCoordsCube);
            for (size_t coord = 0; coord < kCountCoordsCube; ++coord) {
                std::cin >> cube[coord];
            }
            std::cout << space.QueryRange(cube) << "\n";
        } else if (3 == code) {
            should = false;
        }
    }

    return 0;
}
