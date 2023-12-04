// g++ --std=c++20 day4.cpp && cat input.txt | ./a.out
#include "../AoC"

namespace day4
{
using namespace AoC::common;

auto process_line(std::string_view line) -> size_t
{
    using namespace std::views;

    auto card_values = get_front(line | split(':') | drop(1) | transform(make_sv)).value_or(""sv);

    auto winning_numbers_sv = get_front(card_values | split('|') | take(1) | transform(make_sv)).value_or(""sv);
    auto card_numbers_sv = get_front(card_values | split('|') | drop(1) | transform(make_sv)).value_or(""sv);

    auto winning_numbers_view = winning_numbers_sv | split(' ') | filter(is_not_empty_fn()) | transform(make_int);
    auto card_numbers_view = card_numbers_sv | split(' ') | filter(is_not_empty_fn()) | transform(make_int);

    std::set<int> winning_numbers_set(winning_numbers_view.begin(), winning_numbers_view.end());
    std::set<int> card_numbers_set(card_numbers_view.begin(), card_numbers_view.end());

    std::set<int> winners;
    std::ranges::set_intersection(winning_numbers_set, card_numbers_set, std::inserter(winners, winners.begin()));

    return winners.size();
}

auto get_points_part1(std::string_view line) -> int
{
    if (size_t count = process_line(line); count > 0)
    {
        return 1 << (count - 1);
    }
    return 0;
}

struct card_pile_part2
{
    auto add_card(std::string_view line) -> void
    {
        const int current_count = ++card_counts[++line_number];
        if (size_t count = process_line(line); count > 0)
        {
            for (size_t card = 0; card < count; ++card)
            {
                card_counts[line_number + card + 1] += current_count;
            }
        }
    }

    [[nodiscard]] auto get_count() const -> int
    {
        auto map_values = card_counts | std::ranges::views::values;
        return std::accumulate(map_values.begin(), map_values.end(), 0, std::plus<>());
    }

private:
    size_t line_number = 0;
    std::map<size_t, int> card_counts;
};

} // namespace day4

auto main() -> int
{
    std::string line;
    int summe_part1 = 0;
    int summe_part2 = 0;
    day4::card_pile_part2 card_pile;
    while (std::getline(std::cin, line))
    {
        summe_part1 += day4::get_points_part1(line);
        card_pile.add_card(line);
    }
    summe_part2 = card_pile.get_count();
    std::cout << "Sum Part1: " << summe_part1 << '\n';
    std::cout << "Sum Part2: " << summe_part2 << '\n';
    return 0;
}
