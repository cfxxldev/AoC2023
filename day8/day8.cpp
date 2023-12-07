#include "../AoC.hpp"

namespace day8
{
struct card_game
{
    auto parse_line(std::string_view line) -> void
    {
    }
    [[nodiscard]] auto get_part1() const -> auto
    {
        int64_t summe = 0;
        return summe;
    }
    [[nodiscard]] auto get_part2() const -> auto
    {
        int64_t summe = 0;
        return summe;
    }

private:
};
} // namespace day8

// g++ --std=c++20 -O3 -ltbb day8.cpp && cat input.txt | ./a.out
auto main() -> int
{
    std::string line;
    int64_t summe_part1 = 0;
    int64_t summe_part2 = 0;
    day8::card_game card_game;
    while (std::getline(std::cin, line))
    {
        card_game.parse_line(line);
    }
    summe_part1 = card_game.get_part1();
    summe_part2 = card_game.get_part2();
    std::cout << "Sum Part1: " << summe_part1 << '\n';
    std::cout << "Sum Part2: " << summe_part2 << '\n';
    return 0;
}
