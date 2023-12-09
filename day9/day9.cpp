#include "../AoC.hpp"

namespace day9
{
using namespace AoC::common;

auto differences(const std::ranges::range auto &numbers)
{
    using namespace std::views;
    auto values = numbers | slide(2) | transform([](auto arg) {
                      auto begin = arg.begin();
                      auto num1 = *begin;
                      auto num2 = *(++begin);
                      return num2 - num1;
                  });
    return std::vector<int64_t>{values.begin(), values.end()};
}
auto parse_line(std::string_view line)
{
    using namespace std::views;
    namespace ranges = std::ranges;
    auto nums = line | split(' ') | transform(make_int<int64_t>);
    std::vector<int64_t> numbers{nums.begin(), nums.end()};
    std::vector<int64_t> end_numbers;
    end_numbers.push_back(numbers.back());
    std::vector<int64_t> first_numbers;
    first_numbers.push_back(numbers.front());

    for (auto diffs = differences(numbers); !ranges::all_of(diffs, is_equal_fn(0)); diffs = differences(diffs))
    {
        end_numbers.push_back(diffs.back());
        first_numbers.push_back(diffs.front());
    }

    ranges::for_each(first_numbers | reverse | slide(2), [](auto arg) {
        auto begin = arg.begin();
        auto num1 = *begin;
        auto &num2 = *(++begin);
        num2 -= num1;
    });

    return std::make_pair(std::accumulate(end_numbers.begin(), end_numbers.end(), 0l, std::plus<>()),
                          first_numbers.front());
}

} // namespace day9

// g++ --std=c++23 -O3 -ltbb day9.cpp && cat input.txt | ./a.out
auto main() -> int
{
    std::string line;
    int64_t summe_part1 = 0;
    int64_t summe_part2 = 0;
    while (std::getline(std::cin, line))
    {
        auto ret = day9::parse_line(line);
        summe_part1 += ret.first;
        summe_part2 += ret.second;
    }
    std::cout << "Sum Part1: " << summe_part1 << '\n';
    std::cout << "Sum Part2: " << summe_part2 << '\n';
    return 0;
}
