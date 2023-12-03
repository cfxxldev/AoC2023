#include <cctype>
#include <iostream>
#include <numeric>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

using namespace std::literals;

namespace day3
{
struct number_coordinate
{
    int x;
    int y;
    int number;
    auto operator<=>(const number_coordinate &) const = default;
};

struct symbol_map
{
    symbol_map()
    {
        number_map.reserve(140);
    }

    static auto is_digit(char chr) -> bool
    {
        return (chr >= '0' && chr <= '9');
    }

    auto add_line(std::string &&line) -> void
    {
        number_map.emplace_back(std::move(line));
    }

    auto get_at(int x, int y) -> char
    {
        if (x < 0 || y < 0 || y >= number_map.size())
        {
            return '.';
        }
        if (auto &line = number_map.at(y); x < line.length())
        {
            return line.at(x);
        }
        return '.';
    }

    auto is_symbol(int x, int y) -> bool
    {
        if (char chr = get_at(x, y); chr == '.' || is_digit(chr))
        {
            return false;
        }
        return true;
    }

    auto check_adjacent(int x, int y) -> bool
    {
        return is_symbol(x - 1, y - 1) || is_symbol(x, y - 1) || is_symbol(x + 1, y - 1) || is_symbol(x - 1, y) ||
               is_symbol(x + 1, y) || is_symbol(x - 1, y + 1) || is_symbol(x, y + 1) || is_symbol(x + 1, y + 1);
    }

    auto get_line(int y) -> std::string_view
    {
        if (y < 0 || y >= number_map.size())
        {
            return ""sv;
        }
        return number_map.at(y);
    }

    auto get_summe_part1() -> int
    {
        int summe = 0;
        for (int y = 0; y < number_map.size(); ++y)
        {
            int current_number = 0;
            std::string_view line = get_line(y);
            bool part_number = false;
            for (int x = 0; x <= line.length(); ++x)
            {
                if (char current_char = get_at(x, y); is_digit(current_char))
                {

                    if (check_adjacent(x, y))
                    {
                        part_number = true;
                    }
                    current_number *= 10;
                    current_number += current_char - '0';
                }
                else
                {
                    if (part_number)
                    {
                        summe += current_number;
                    }
                    current_number = 0;
                    part_number = false;
                }
            }
        }
        return summe;
    }

    auto get_number_at(int x, int y) -> std::optional<number_coordinate>
    {
        char temp = get_at(x, y);
        if (!is_digit(temp))
        {
            return {};
        }

        number_coordinate coord{.x = x, .y = y, .number = 0};
        for (int temp_x = x - 1; is_digit(get_at(temp_x, coord.y)); --temp_x)
        {
            coord.x = temp_x;
        }

        for (int temp_x = coord.x; is_digit(get_at(temp_x, coord.y)); ++temp_x)
        {
            coord.number *= 10;
            coord.number += get_at(temp_x, coord.y) - '0';
        }
        return coord;
    }

    auto get_gear_ratio(int x, int y) -> int
    {
        if (get_at(x, y) != '*')
        {
            return 0;
        }

        std::set<number_coordinate> coords;
        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                if (auto coord = get_number_at(x + dx, y + dy); coord.has_value())
                {
                    coords.insert(coord.value());
                    if (coords.size() > 2)
                    {
                        return 0;
                    }
                }
            }
        }

        if (coords.size() == 2)
        {
            return std::accumulate(
                coords.begin(),
                coords.end(),
                1,
                [](int current_value, const number_coordinate &coord) -> int { return current_value * coord.number; });
        }
        return 0;
    }

    auto get_summe_part2() -> int
    {
        int summe = 0;
        for (int y = 0; y < number_map.size(); ++y)
        {
            std::string_view line = get_line(y);
            for (int x = 0; x <= line.length(); ++x)
            {
                summe += get_gear_ratio(x, y);
            }
        }
        return summe;
    }

private:
    std::vector<std::string> number_map;
};


} // namespace day3

auto main() -> int
{
    std::string line;
    day3::symbol_map symbols;
    while (std::getline(std::cin, line))
    {
        symbols.add_line(std::move(line));
    }
    int summe_part1 = symbols.get_summe_part1();
    int summe_part2 = symbols.get_summe_part2();
    std::cout << "Sum Part1: " << summe_part1 << '\n';
    std::cout << "Sum Part2: " << summe_part2 << '\n';
    return 0;
}
