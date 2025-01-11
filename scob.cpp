#include <iostream>
#include <stack>
#include <string>

std::string check_bracket_sequence(const std::string &input) {
  const std::string opens = "([{";
  const std::string closes = ")]}";
  std::stack<char> stack;
  for (size_t index = 0; index < input.size(); ++index) {
    char current_char = input[index];

    if (closes.find(current_char) != std::string::npos) {
      size_t close_index = closes.find(current_char);

      if (!stack.empty() && stack.top() == opens[close_index]) {
        stack.pop();
      } else {
        return std::to_string(index);
      }
    } else {
      stack.push(current_char);
    }
  }

  if (!stack.empty()) {
    return std::to_string(input.size());
  }

  return "CORRECT";
}



int main() {
  std::string input;
  std::cin >> input;
  std::cout << check_bracket_sequence(input) << std::endl;
  return 0;
}
