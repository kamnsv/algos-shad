#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

int main() {
  const int MAX_SIZE = 1000;
  const int32_t MAX_VALUE = 1'000'000'000;
  size_t sequence_size;
  std::cin >> sequence_size;

  if (sequence_size == 0 || sequence_size > MAX_SIZE) {
    return 1;
  }

  std::vector<int32_t> sequence;
  sequence.reserve(sequence_size);
  int32_t value;

  for (size_t i = 0; i < sequence_size; ++i) {
    std::cin >> value;
    if (value > MAX_VALUE) {
      return 2;
    }
    if (!sequence.empty() && value == sequence.back()) {
      continue;
    }
    sequence.push_back(value);
  }

  if (sequence.size() <= 1) {
    for (const auto &val : sequence) {
      std::cout << val << " ";
    }
    std::cout << std::endl;
    return 0;
  }

  const size_t size = sequence.size();
  std::vector<size_t> dp(size, 1);

  for (size_t current_index = 1; current_index < size; ++current_index) {
    for (size_t previous_index = 0; previous_index < current_index;
         ++previous_index) {
      if ((sequence[previous_index] < sequence[current_index] &&
           (previous_index == 0 ||
            sequence[previous_index - 1] > sequence[previous_index])) ||
          (sequence[previous_index] > sequence[current_index] &&
           (previous_index == 0 ||
            sequence[previous_index - 1] < sequence[previous_index]))) {
        dp[current_index] = std::max(dp[current_index], dp[previous_index] + 1);
      }
    }
  }

  const size_t max_length = *std::max_element(dp.begin(), dp.end());
  std::vector<int32_t> result;
  size_t current_position = 0;

  while (current_position < size) {
    if (std::count(dp.begin() + current_position, dp.end(),
                   dp[current_position]) == 1) {
      result.push_back(sequence[current_position]);
      ++current_position;
      continue;
    }

    if (max_length == dp[current_position]) {
      size_t max_pos = std::find(dp.begin(), dp.end(), max_length) - dp.begin();
      size_t candidate_count = std::count(dp.begin(), dp.end(), max_length);

      for (size_t offset = 0; offset < candidate_count; ++offset) {
        int candidate = sequence[max_pos + offset];

        if (result.size() > 1) {
          int last = result.back();
          int second_last = result[result.size() - 2];

          if ((second_last < last && last > candidate) ||
              (second_last > last && last < candidate)) {
            result.push_back(candidate);
            break;
          }
        } else {
          result.push_back(candidate);
          break;
        }
      }
      break;
    }

    size_t next_position =
        std::find(dp.begin(), dp.end(), dp[current_position] + 1) - dp.begin();
    size_t candidate_count =
        std::count(dp.begin(), dp.end(), dp[current_position] + 1);

    for (size_t offset = 0; offset < candidate_count; ++offset) {
      if (next_position + offset <= current_position) {
        continue;
      }
      if ((result.back() < sequence[current_position] &&
           sequence[current_position] > sequence[next_position + offset]) ||
          (result.back() > sequence[current_position] &&
           sequence[current_position] < sequence[next_position + offset])) {
        result.push_back(sequence[current_position]);
        current_position = next_position + offset - 1;
        break;
      }
    }
    ++current_position;
  }

  for (const auto &val : result) {
    std::cout << val << " ";
  }
  std::cout << std::endl;

  return 0;
}
