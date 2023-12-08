#include "../AoC.hpp"

namespace day8
{
struct node
{
    uint16_t left{};
    uint16_t right{};
};
constexpr const uint16_t last_char_mask = 0x1F;
constexpr auto to_index(std::string_view node_name) -> uint16_t
{
    uint16_t result{};
    for (auto chr : node_name)
    {
        result <<= 5;
        result |= (chr - 'A') & last_char_mask;
    }
    return result;
}
constinit const uint16_t AAA = to_index("AAA"sv);
constinit const uint16_t ZZZ = to_index("ZZZ"sv);
constinit const uint16_t XXA = to_index("A"sv);
constinit const uint16_t XXZ = to_index("Z"sv);

struct map_game
{
    auto parse_line(std::string_view line) -> void
    {
        if (line.empty())
        {
            return;
        }
        if (line.length() == 16 && line.back() == ')')
        {
            auto node_name = line.substr(0, 3);
            auto left_node = line.substr(7, 3);
            auto right_node = line.substr(12, 3);
            nodes[to_index(node_name)] = {to_index(left_node), to_index(right_node)};
            return;
        }

        if (auto first = line.front(); first == 'L' || first == 'R')
        {
            std::ranges::copy(line, std::back_inserter(directions));
            return;
        }
    }
    [[nodiscard]] auto get_part1() const -> auto
    {
        uint64_t summe = 0;
        walk_result result{.last_node = AAA};
        result = walk_nodes(result.last_node);
        summe += result.steps;
        return summe;
    }
    [[nodiscard]] auto get_part2() const -> auto
    {
        using namespace std::views;
        uint64_t summe = 0;
        auto starting_nodes = get_starting_positions();

        auto find_first_Z = [this](auto &arg) noexcept {
            while ((next_step(get_direction(arg.steps), arg) & last_char_mask) != XXZ)
            {
            }
        };

        std::ranges::for_each(starting_nodes, find_first_Z);
        summe = std::accumulate(starting_nodes.begin(),starting_nodes.end(), 1ul, [](auto out, auto &arg){
            return std::lcm(out,arg.steps);
        });

        return summe;
    }

private:
    struct walk_result
    {
        uint64_t steps{};
        uint16_t last_node{};
    };
    [[nodiscard]] auto get_direction_step(auto step) const -> uint64_t
    {
        return step % directions.size();
    }
    [[nodiscard]] auto get_direction(auto step) const -> char
    {
        return directions.at(get_direction_step(step));
    }
    auto next_step(char dir, walk_result &result) const -> uint16_t
    {
        auto node = nodes.at(result.last_node);
        result.last_node = dir == 'L' ? node.left : node.right;
        ++result.steps;
        return result.last_node;
    }
    [[nodiscard]] auto walk_nodes(uint16_t start) const -> walk_result
    {
        walk_result result{.steps = 0, .last_node = start};
        while (result.last_node != ZZZ)
        {
            next_step(get_direction(result.steps), result);
        }
        return result;
    }
    [[nodiscard]] auto get_starting_positions() const -> std::vector<walk_result>
    {
        std::vector<walk_result> result;
        for (const auto &[node,_]:nodes)
        {
            if ((node & last_char_mask) == XXA)
            {
                result.emplace_back(0, node);
            }
        }
        return result;
    }
    std::vector<char> directions;
    std::map<uint16_t, node> nodes;
};
} // namespace day8

// g++ --std=c++23 -O3 -ltbb day8.cpp && cat input.txt | ./a.out
auto main() -> int
{
    std::string line;
    uint64_t summe_part1 = 0;
    uint64_t summe_part2 = 0;
    day8::map_game map_game;
    while (std::getline(std::cin, line))
    {
        map_game.parse_line(line);
    }
    summe_part1 = map_game.get_part1();
    summe_part2 = map_game.get_part2();
    std::cout << "Sum Part1: " << summe_part1 << '\n';
    std::cout << "Sum Part2: " << summe_part2 << '\n';
    return 0;
}
