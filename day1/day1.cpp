#include <array>
#include <cctype>
#include <iostream>
#include <string>
#include <string_view>

using namespace std::literals;

namespace {
struct digit_string {
  std::string_view find;
  int digit;
};
struct digit_pos {
  int digit;
  size_t pos;
};

constexpr auto npos = std::string_view::npos;
auto find_digit_pos_part1(std::string_view base, bool last) -> digit_pos {
  static constexpr auto digits = "0123456789"sv;
  if (auto pos = last ? base.find_last_of(digits) : base.find_first_of(digits);
      pos != npos) {
    return digit_pos{.digit = base.at(pos) - '0', .pos = pos};
  }
  return digit_pos{.digit = 0, .pos = npos};
};

auto getNumber_part1(std::string_view line) -> int {
  return find_digit_pos_part1(line, false).digit * 10 +
         find_digit_pos_part1(line, true).digit;
}

auto find_digit_pos_part2(std::string_view base, bool last) -> digit_pos {
  static constexpr std::array<digit_string, 9> digit_strings = {
      digit_string{"one"sv, 1},
      {"two"sv, 2},
      {"three"sv, 3},
      {"four"sv, 4},
      {"five"sv, 5},
      {"six"sv, 6},
      {"seven"sv, 7},
      {"eight"sv, 8},
      {"nine"sv, 9}};
  digit_pos digit = find_digit_pos_part1(base, last);
  for (auto f : digit_strings) {
    if (auto pos = last ? base.rfind(f.find) : base.find(f.find); pos != npos) {
      if ((last && pos > digit.pos) || (!last && pos < digit.pos) ||
          (digit.pos == npos)) {
        digit = {.digit = f.digit, .pos = pos};
      }
    }
  }
  return digit;
};
auto getNumber_part2(std::string_view line) -> int {
  return find_digit_pos_part2(line, false).digit * 10 +
         find_digit_pos_part2(line, true).digit;
}
} // namespace

auto main() -> int {
  std::string line;
  int summe_part1 = 0;
  int summe_part2 = 0;
  while (std::getline(std::cin, line)) {
    summe_part1 += getNumber_part1(line);
    summe_part2 += getNumber_part2(line);
  }
  std::cout << "Sum Part1: " << summe_part1 << '\n';
  std::cout << "Sum Part2: " << summe_part2 << '\n';
  return 0;
}
