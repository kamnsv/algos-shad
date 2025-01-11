#include <vector>
#include <iostream>
#include <cmath>
#include <array>
#include <algorithm>
#include <utility>
#include <tuple>

class HalfPlaneCounter {
public:
    explicit HalfPlaneCounter(int max_coord, size_t bucket_count)
        : kMaxCoord(max_coord), kBucketCount(bucket_count) {
        bucket_size_ = (kMaxCoord + kBucketCount - 1) / kBucketCount;
        buckets_.resize(kBucketCount * kBucketCount);
    }

    void Build(const std::vector<std::pair<int, int>>& points) {
        for (const auto& point : points) {
            size_t bucket_x = std::min(static_cast<size_t>(point.first / bucket_size_), 
                                                               kBucketCount - 1);
            size_t bucket_y = std::min(static_cast<size_t>(point.second / bucket_size_),
                                                               kBucketCount - 1);
            buckets_[bucket_x * kBucketCount + bucket_y].push_back(point);
        }
    }

    size_t Count(int coef_x, int coef_y, int coef_c) const {
        size_t count = 0;

        for (size_t bucket_x = 0; bucket_x < kBucketCount; ++bucket_x) {
            for (size_t bucket_y = 0; bucket_y < kBucketCount; ++bucket_y) {
                auto corners = Corners(bucket_x, bucket_y);
                if (Completely(corners, coef_x, coef_y, coef_c)) {
                    count += buckets_[bucket_x * kBucketCount + bucket_y].size();
                } else if (!Out(corners, coef_x, coef_y, coef_c)) {
                    count += Points(bucket_x, bucket_y, coef_x, coef_y, coef_c);
                }
            }
        }

        return count;
    }

private:
    const int kMaxCoord;
    const size_t kBucketCount;
    int bucket_size_;
    std::vector<std::vector<std::pair<int, int>>> buckets_;

    bool Completely(const std::array<std::pair<int, int>, 4>& corners,
                    int coef_x, int coef_y, int coef_c) const {
        return std::all_of(corners.begin(), corners.end(), [&](const auto& corner) {
            return coef_x * corner.first + coef_y * corner.second + coef_c >= 0;
        });
    }

    bool Out(const std::array<std::pair<int, int>, 4>& corners,
             int coef_x, int coef_y, int coef_c) const {
        return std::all_of(corners.begin(), corners.end(), [&](const auto& corner) {
            return coef_x * corner.first + coef_y * corner.second + coef_c < 0;
        });
    }

    size_t Points(size_t bucket_x, size_t bucket_y,
                  int coef_x, int coef_y, int coef_c) const {
        size_t count = 0;
        const auto& bucket = buckets_[bucket_x * kBucketCount + bucket_y];
        for (const auto& point : bucket) {
            if (coef_x * point.first + coef_y * point.second + coef_c >= 0) {
                ++count;
            }
        }
        return count;
    }

    std::array<std::pair<int, int>, 4> Corners(size_t bucket_x, size_t bucket_y) const {
        int x_min = bucket_x * bucket_size_;
        int y_min = bucket_y * bucket_size_;
        int x_max = (bucket_x + 1) * bucket_size_;
        int y_max = (bucket_y + 1) * bucket_size_;

        return {{{x_min, y_min},
                 {x_min, y_max},
                 {x_max, y_min},
                 {x_max, y_max}}};
    }
};

int main() {
    size_t point_count, query_count;
    int max_coord;

    std::cin >> point_count >> query_count >> max_coord;

    std::vector<std::pair<int, int>> points(point_count);
    for (auto& point : points) {
        std::cin >> point.first >> point.second;
    }

    std::vector<std::tuple<int, int, int>> queries(query_count);
    for (auto& query : queries) {
        std::cin >> std::get<0>(query) >> std::get<1>(query) >> std::get<2>(query);
    }

    size_t bucket_count = static_cast<size_t>(std::sqrt(max_coord));
    HalfPlaneCounter counter(max_coord, bucket_count);

    counter.Build(points);

    for (const auto& query : queries) {
        std::cout << counter.Count(std::get<0>(query),
                                   std::get<1>(query),
                                   std::get<2>(query)) << "\n";
    }

    return 0;
}
