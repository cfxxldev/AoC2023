#include "../AoC.hpp"

namespace day6
{
using namespace AoC::common;
struct race_card
{
    auto parse_line(std::string_view line) -> void
    {
        using namespace std::ranges::views;
        if (line.starts_with("Time:"))
        {
            auto ints = line | drop(5) | split(' ') | filter(is_not_empty_fn()) | transform(make_int<int>);
            times.insert(times.end(), ints.begin(), ints.end());
            super_time = to_int<int64_t>(range_to_string(line | drop(5) | filter(is_not_char_fn(' '))));
        }
        if (line.starts_with("Distance:"))
        {
            auto ints = line | drop(9) | split(' ') | filter(is_not_empty_fn()) | transform(make_int<int>);
            distances.insert(distances.end(), ints.begin(), ints.end());
            super_distance = to_int<int64_t>(range_to_string(line | drop(9) | filter(is_not_char_fn(' '))));
        }
    }
    static auto filter_distance_fn(std::integral auto min_distance) -> auto
    {
        return [min_distance](std::integral auto distance) { return distance > min_distance; };
    }
    static auto to_distance_fn(std::integral auto max_time) -> auto
    {
        return [max_time](std::integral auto time) { return (max_time - time) * time; };
    }
    static auto possible_times(std::integral auto max_time, std::integral auto max_distance) -> auto
    {
        using namespace std::ranges::views;
        return std::ranges::count_if(iota(0, max_time) | transform(to_distance_fn(max_time)),
                                     filter_distance_fn(max_distance));
    }
    [[nodiscard]] auto get_part1() const -> auto
    {
        long result = 1;
        for (int idx = 0; idx < times.size(); ++idx)
        {
            result *= possible_times(times.at(idx), distances.at(idx));
        }
        return result;
    }
    [[nodiscard]] auto get_part2() const -> auto
    {
        return possible_times(super_time, super_distance);
    }

private:
    std::vector<int> times;
    std::vector<int> distances;
    int64_t super_time{};
    int64_t super_distance{};
};

} // namespace day6

// g++ --std=c++20 -O3 -ltbb day5.cpp && cat input.txt | ./a.out
auto main() -> int
{
    std::string line;
    int64_t summe_part1 = 0;
    int64_t summe_part2 = 0;
    day6::race_card race_card;
    while (std::getline(std::cin, line))
    {
        race_card.parse_line(line);
    }
    summe_part1 = race_card.get_part1();
    summe_part2 = race_card.get_part2();
    std::cout << "Sum Part1: " << summe_part1 << '\n';
    std::cout << "Sum Part2: " << summe_part2 << '\n';
    return 0;
}
