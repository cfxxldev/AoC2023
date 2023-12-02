#include <array>
#include <cctype>
#include <cstddef>
#include <functional>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>

using namespace std::literals;
namespace sview = std::views;

namespace
{

struct color_count{
  int red;
  int green;
  int blue;
};

auto processLine(std::string_view line, const std::function<bool(const color_count&)> &fn_check) -> int
{
    auto to_sv = [](std::ranges::range auto &&view)
    {
      return std::string_view{view.begin(),view.end()};
    };

    auto split_line = line | sview::split(':') | sview::take(2);

    auto beginn = split_line.begin();
    auto game_id = (*beginn | sview::split(' ') | sview::drop(1) | sview::take(1) | sview::transform(to_sv)).front();
    auto set_list = *(++beginn) | sview::split(';');

    for (auto set : set_list)
    {
        color_count colors {0,0,0};
        auto pull_list = set | sview::split(',');
        for (auto pull : pull_list)
        {
          auto count = (pull | sview::drop_while(isblank) | sview::split(' ') | sview::take(1) | sview::transform(to_sv)).front();
          auto color = (pull | sview::drop_while(isblank) | sview::split(' ') | sview::drop(1) | sview::take(1) | sview::transform(to_sv)).front();
          if (color == "red")
          {
            colors.red += std::stoi(std::string(count.begin(),count.end()));
          }
          if (color == "green")
          {
            colors.green += std::stoi(std::string(count.begin(),count.end()));
          }
          if (color == "blue")
          {
            colors.blue += std::stoi(std::string(count.begin(),count.end()));
          }
        }
        if(!fn_check(colors))
        {
          return 0;
        }
    }
    return std::stoi(std::string{game_id.begin(), game_id.end()});
}

auto getPossibleID_part1(std::string_view line) -> int
{
    static constexpr color_count target_counts{.red = 12, .green = 13, .blue = 14};
    return processLine(line, [](const color_count &colors){
        return colors.red <= target_counts.red &&
        colors.green <= target_counts.green &&
        colors.blue <= target_counts.blue;
    });
}

auto getPower_part2(std::string_view line) -> int
{
    color_count min_counts{.red = 0, .green = 0, .blue = 0};
    processLine(line, [&min_counts](const color_count &colors){
        min_counts.red = std::max(colors.red,min_counts.red);
        min_counts.green = std::max(colors.green,min_counts.green);
        min_counts.blue = std::max(colors.blue,min_counts.blue);
        return true;
        });
    return min_counts.red * min_counts.green * min_counts.blue;
}

} // namespace

auto main() -> int
{
    std::string line;
    int summe_part1 = 0;
    int summe_part2 = 0;
    while (std::getline(std::cin, line))
    {
        summe_part1 += getPossibleID_part1(line);
        summe_part2 += getPower_part2(line);
    }
    std::cout << "Sum Part1: " << summe_part1 << '\n';
    std::cout << "Sum Part2: " << summe_part2 << '\n';
    return 0;
}
