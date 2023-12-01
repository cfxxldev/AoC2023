#include <iostream>
#include <string>
#include <string_view>
#include <cctype>
#include <array>

using namespace std::literals;

int getNumber(std::string_view line)
{
    int ret = 0;
    constexpr auto digits = "0123456789"sv;
    constexpr auto npos = std::string_view::npos;

    if(auto pos = line.find_first_of(digits); pos != npos)
    {
        ret = line.at(pos) - '0';
    }

    if(auto pos = line.find_last_of(digits); pos != npos)
    {
        ret *= 10;
        ret += line.at(pos) - '0';
    }

    return ret;
}

int getNumberWithNames(std::string_view line)
{
    int ret = 0;
    struct digit_string{
        std::string_view find;
        int digit;
    };
    static constexpr std::array digit_strings{
        digit_string{"1"sv,1},
        digit_string{"2"sv,2},
        digit_string{"3"sv,3},
        digit_string{"4"sv,4},
        digit_string{"5"sv,5},
        digit_string{"6"sv,6},
        digit_string{"7"sv,7},
        digit_string{"8"sv,8},
        digit_string{"9"sv,9},
        digit_string{"0"sv,0},
        digit_string{"one"sv,1},
        digit_string{"two"sv,2},
        digit_string{"three"sv,3},
        digit_string{"four"sv,4},
        digit_string{"five"sv,5},
        digit_string{"six"sv,6},
        digit_string{"seven"sv,7},
        digit_string{"eight"sv,8},
        digit_string{"nine"sv,9}
    };

    auto get_first_digit = [] (std::string_view base)
    {
        size_t current_pos = std::string::npos;
        int current_digit = 0;
        for(auto f: digit_strings)
        {
            if(auto pos = base.find(f.find); pos != std::string::npos)
            {
                if(pos < current_pos || current_pos == std::string::npos)
                {
                    current_pos = pos;
                    current_digit = f.digit;
                }
            }
        }
        return current_digit;
    };

    auto get_last_digit = [] (std::string_view base)
    {
        size_t current_pos = std::string::npos;
        int current_digit = 0;
        for(auto f: digit_strings)
        {
            if(auto pos = base.rfind(f.find); pos != std::string::npos)
            {
                if(pos > current_pos || current_pos == std::string::npos)
                {
                    current_pos = pos;
                    current_digit = f.digit;
                }
            }
        }
        return current_digit;
    };

    ret = get_first_digit(line) * 10 + get_last_digit(line);
    return ret;
}

int main()
{
    std::string line;
    int summe_part1 = 0;
    int summe_part2 = 0;
    while (std::getline(std::cin, line))
    {
        summe_part1 += getNumber(line);
        summe_part2 += getNumberWithNames(line);
    }
    std::cout << "Sum Part1: " << summe_part1 << '\n';
    std::cout << "Sum Part2: " << summe_part2 << '\n';
    return 0;
}
