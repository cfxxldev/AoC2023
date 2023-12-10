#include "../AoC.hpp"

namespace day10
{
using namespace AoC::common;
constexpr const auto UpDown = "UD"sv;
constexpr const auto UpRight = "UR"sv;
constexpr const auto UpLeft = "UL"sv;
constexpr const auto DownRight = "DR"sv;
constexpr const auto DownLeft = "DL"sv;
constexpr const auto LeftRight = "LR"sv;

auto map_pipe(char pipe)
{
    using namespace matchit;
    return match(pipe)(pattern | '|' = UpDown,
                       pattern | '-' = LeftRight,
                       pattern | 'L' = UpRight,
                       pattern | 'J' = UpLeft,
                       pattern | '7' = DownLeft,
                       pattern | 'F' = DownRight,
                       pattern | _ = ""sv);
}
auto opposite(char dir) -> char
{
    using namespace matchit;
    return match(
        dir)(pattern | 'U' = 'D', pattern | 'D' = 'U', pattern | 'L' = 'R', pattern | 'R' = 'L', pattern | _ = dir);
}
auto left(char dir)
{
    using namespace matchit;
    return match(
        dir)(pattern | 'U' = 'L', pattern | 'D' = 'R', pattern | 'L' = 'D', pattern | 'R' = 'U', pattern | _ = dir);
}
auto right(char dir)
{
    return left(opposite(dir));
}
auto is_up(std::string_view dir)
{
    return dir.contains('U');
}
auto is_down(std::string_view dir)
{
    return dir.contains('D');
}
auto is_left(std::string_view dir)
{
    return dir.contains('L');
}
auto is_rigt(std::string_view dir)
{
    return dir.contains('R');
}
struct coordinate
{
    int64_t x_pos{};
    int64_t y_pos{};

    [[nodiscard]] auto get_next(char dir) const
    {
        using namespace matchit;
        return match(dir)(pattern | 'U' = coordinate{.x_pos = x_pos, .y_pos = y_pos - 1},
                          pattern | 'D' = coordinate{.x_pos = x_pos, .y_pos = y_pos + 1},
                          pattern | 'L' = coordinate{.x_pos = x_pos - 1, .y_pos = y_pos},
                          pattern | 'R' = coordinate{.x_pos = x_pos + 1, .y_pos = y_pos},
                          pattern | _ = *this);
    }
    auto operator<=>(const coordinate &) const = default;
};
struct pipe_node
{
    coordinate pos;
    char dir{};
    char from{};
    auto operator<=>(const pipe_node &) const = default;
};
struct pipes
{
    auto get_at(int64_t x_pos, int64_t y_pos)
    {
        return mappable(pipe_map.get_at(x_pos, y_pos).value_or('O')).map(map_pipe);
    }
    auto get_at(coordinate coord)
    {
        return get_at(coord.x_pos, coord.y_pos);
    }
    auto get_start_pipe()
    {
        auto from_top = get_at(start_pos.x_pos, start_pos.y_pos - 1).map(is_down).value();
        auto from_left = get_at(start_pos.x_pos - 1, start_pos.y_pos).map(is_rigt).value();
        auto from_right = get_at(start_pos.x_pos + 1, start_pos.y_pos).map(is_left).value();
        if (from_top)
        {
            if (from_left)
            {
                return 'J';
            }
            if (from_right)
            {
                return 'L';
            }
            return '|';
        }
        if (from_left)
        {
            if (from_right)
            {
                return '-';
            }
            return '7';
        }
        return 'F';
    }
    auto add_line(const std::string &line) -> std::string &
    {
        auto &new_line = pipe_map.add_line(line);
        if (auto pos_x = new_line.find('S'); pos_x != std::string::npos)
        {
            start_pos = {.x_pos = static_cast<int64_t>(pos_x),
                         .y_pos = static_cast<int64_t>(pipe_map.lines().size() - 1)};
            new_line[pos_x] = get_start_pipe();
        }
        return new_line;
    }
    auto get_next_dir(coordinate coord, char from) -> char
    {
        auto pipe = get_at(coord).value();
        for (auto dir : pipe)
        {
            if (dir != opposite(from))
            {
                return dir;
            }
        }
        std::unreachable();
        return opposite(from);
    }
    auto next_node(const pipe_node &node) -> pipe_node
    {
        pipe_node result;
        result.pos = node.pos.get_next(node.dir);
        result.from = node.dir;
        result.dir = get_next_dir(result.pos, node.dir);
        return result;
    }
    auto get_part1()
    {
        auto start_pipe = get_at(start_pos).value();

        int64_t count = 0;
        for (pipe_node node = next_node({.pos = start_pos, .dir = start_pipe[0]}); node.pos != start_pos;
             node = next_node(node))
        {
            ++count;
        }
        return (count / 2) + (count % 2);
    }
    auto remove_unused_pipes(const std::set<coordinate> &used_pipes)
    {
        for (auto [pos_y, line] : pipe_map.lines() | std::views::enumerate)
        {
            for (auto [pos_x, pipe] : line | std::views::enumerate)
            {
                if (!used_pipes.contains({pos_x, pos_y}))
                {
                    pipe = '.';
                }
            }
        }
    }
    auto mark_left_right()
    {
        auto start_pipe = get_at(start_pos).value();
        for (pipe_node node = next_node({.pos = start_pos, .dir = start_pipe[0]}); node.pos != start_pos;
             node = next_node(node))
        {
            for (auto check_dir : {node.dir, node.from})
            {
                auto pos = node.pos.get_next(left(check_dir));
                if (pipe_map.get_at(pos.x_pos, pos.y_pos).value_or('.') == '.')
                {
                    pipe_map.set_at(pos.x_pos, pos.y_pos, 'l');
                }
                pos = node.pos.get_next(right(check_dir));
                if (pipe_map.get_at(pos.x_pos, pos.y_pos).value_or('.') == '.')
                {
                    pipe_map.set_at(pos.x_pos, pos.y_pos, 'r');
                }
            }
        }
    }
    auto fill_left_right(char l_or_r, char outside)
    {
        bool replaced = true;
        while (replaced)
        {
            replaced = false;
            for (auto [pos_y, line] : pipe_map.lines() | std::views::enumerate)
            {
                for (auto [pos_x, pipe] : line | std::views::enumerate)
                {
                    if (pipe == '.')
                    {
                        if (pipe_map.get_at(pos_x - 1, pos_y).value_or(outside) == l_or_r ||
                            pipe_map.get_at(pos_x + 1, pos_y).value_or(outside) == l_or_r ||
                            pipe_map.get_at(pos_x, pos_y - 1).value_or(outside) == l_or_r ||
                            pipe_map.get_at(pos_x, pos_y + 1).value_or(outside) == l_or_r)
                        {
                            pipe_map.set_at(pos_x, pos_y, l_or_r);
                            replaced = true;
                        }
                    }
                }
            }
        }
    }
    auto detect_outside()
    {
        for (const auto &line : pipe_map.lines())
        {
            for (auto pipe : line)
            {
                if (pipe == 'r' || pipe == 'l')
                {
                    return pipe;
                }
                if (pipe != '.')
                {
                    continue;
                }
            }
        }
        std::unreachable();
        return ' ';
    }
    auto get_part2()
    {
        auto start_pipe = get_at(start_pos).value();

        std::set<coordinate> used_pipes;
        used_pipes.insert(start_pos);
        for (pipe_node node = next_node({.pos = start_pos, .dir = start_pipe[0]}); node.pos != start_pos;
             node = next_node(node))
        {
            used_pipes.insert(node.pos);
        }

        remove_unused_pipes(used_pipes);
        mark_left_right();
        auto outside = detect_outside();
        auto inside = outside == 'l' ? 'r' : 'l';
        fill_left_right(outside, outside);
        fill_left_right(inside, outside);

        auto counts = pipe_map.lines() | std::views::transform([inside](const auto &line) {
                          return std::ranges::count_if(line, is_equal_fn(inside));
                      });
        return std::accumulate(counts.begin(), counts.end(), 0l, std::plus<>());
    }
private:
    coordinate start_pos;
    chars_map pipe_map;
};
} // namespace day10

// g++ --std=c++23 -O3 -ltbb day10.cpp && cat input.txt | ./a.out
auto main() -> int
{
    std::string line;
    int64_t summe_part1 = 0;
    int64_t summe_part2 = 0;
    day10::pipes pipes;
    while (std::getline(std::cin, line))
    {
        pipes.add_line(line);
    }
    summe_part1 = pipes.get_part1();
    summe_part2 = pipes.get_part2();
    std::cout << "Sum Part1: " << summe_part1 << '\n';
    std::cout << "Sum Part2: " << summe_part2 << '\n';
    return 0;
}
