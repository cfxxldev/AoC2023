#include <cctype>
#include <charconv>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>

using namespace std::literals;

namespace day2
{

struct color_count
{
    int red;
    int green;
    int blue;
};

auto make_sv(std::ranges::subrange<const char *> val) -> std::string_view
{
    return {val.begin(), val.end()};
};

auto make_int(std::ranges::subrange<const char *> val) -> int
{
    int result = 0;
    auto [_, ec] = std::from_chars(val.begin(), val.end(), result);
    return result;
};

auto get_front(std::ranges::range auto &&view) -> decltype(auto)
{
    if (!view.empty())
    {
        return std::optional(view.front());
    }
    return std::optional<decltype(view.front())>{};
}

auto processLine(std::string_view line, const std::function<bool(const color_count &)> &fn_check) -> std::optional<int>
{
    using namespace std::views;

    auto match = [](std::string_view match, std::ranges::range auto &input_view, std::ranges::range auto &result_view) {
        return get_front(input_view | filter([&match](std::string_view val) { return val == match; }) |
                         transform([&result_view](std::ranges::subrange<const char *> val) {
                             return get_front(result_view).value_or(0);
                         }));
    };

    auto split_line = line | split(':') | take(2);
    auto game_id = get_front(split_line | take(1)).value_or(""sv) | split(' ') | drop(1) | transform(make_int);
    auto set_list = get_front(split_line | drop(1)).value_or(""sv) | split(';');

    for (const auto& set : set_list)
    {
        color_count colors{.red = 0, .green = 0, .blue = 0};
        auto pull_list = set | split(',');
        for (const auto& pull : pull_list)
        {
            auto count = pull | drop_while(isblank) | split(' ') | take(1) | transform(make_int);
            auto color = pull | drop_while(isblank) | split(' ') | drop(1) | take(1) | transform(make_sv);

            colors.red += match("red", color, count).value_or(0);
            colors.green += match("green", color, count).value_or(0);
            colors.blue += match("blue", color, count).value_or(0);
        }
        if (!fn_check(colors))
        {
            return 0;
        }
    }
    return get_front(game_id);
}

auto getPossibleID_part1(std::string_view line) -> std::optional<int>
{
    static constexpr color_count target_counts{.red = 12, .green = 13, .blue = 14};
    return processLine(line, [](const color_count &colors) {
        return colors.red <= target_counts.red && colors.green <= target_counts.green &&
               colors.blue <= target_counts.blue;
    });
}

auto getPower_part2(std::string_view line) -> std::optional<int>
{
    color_count min_counts{.red = 0, .green = 0, .blue = 0};
    processLine(line, [&min_counts](const color_count &colors) {
        min_counts.red = std::max(colors.red, min_counts.red);
        min_counts.green = std::max(colors.green, min_counts.green);
        min_counts.blue = std::max(colors.blue, min_counts.blue);
        return true;
    });
    return min_counts.red * min_counts.green * min_counts.blue;
}

} // namespace day2

auto main() -> int
{
    std::string line;
    int summe_part1 = 0;
    int summe_part2 = 0;
    while (std::getline(std::cin, line))
    {
        summe_part1 += day2::getPossibleID_part1(line).value_or(0);
        summe_part2 += day2::getPower_part2(line).value_or(0);
    }
    std::cout << "Sum Part1: " << summe_part1 << '\n';
    std::cout << "Sum Part2: " << summe_part2 << '\n';
    return 0;
}
