#ifndef _AoC_included_
#define _AoC_included_

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <execution>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include "matchit.h"

using namespace std::literals;

namespace AoC::common
{
auto is_digit(char chr) -> bool
{
    return (chr >= '0' && chr <= '9');
}

auto make_sv(std::ranges::subrange<const char *> val) -> std::string_view
{
    return {val.begin(), val.end()};
};

template <typename T = int>
auto make_int(std::ranges::subrange<const char *> val) -> T
{
    T result = 0;
    [[maybe_unused]] auto [_, ec] = std::from_chars(val.begin(), val.end(), result);
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

auto is_not_empty_fn() -> auto
{
    return [](const auto &tmp_v) -> bool { return !tmp_v.empty(); };
}

auto is_char_fn(char chr) -> auto
{
    return [chr](char tmp) -> bool { return chr == tmp; };
}

auto is_not_char_fn(char chr) -> auto
{
    return [chr](char tmp) -> bool { return chr != tmp; };
}

auto is_equal_fn(auto value) -> auto
{
    return [value](auto tmp) -> bool { return value == tmp; };
}

auto is_not_equal_fn(auto value) -> auto
{
    return [value](auto tmp) -> bool { return value != tmp; };
}

auto match(auto arg1, auto arg2, auto ret) -> std::optional<decltype(ret)>
{
    if (arg1 == arg2)
    {
        return ret;
    }
    return {};
}

template <typename T = int>
auto to_int(std::string_view val) -> T
{
    T result = 0;
    [[maybe_unused]] auto [_, ec] = std::from_chars(val.begin(), val.end(), result);
    return result;
};

auto range_to_string(std::ranges::range auto &&view)
{
    std::string result;
    for (const char chr : view)
    {
        result.push_back(chr);
    }
    return result;
}

auto print_range(const std::ranges::range auto &numbers)
{
    for(auto num:numbers)
    {
        std::cout << num << ' ';
    }
    std::cout << '\n';
}

template <typename T>
struct mappable
{
    explicit mappable(T val) : value_(val){};
    mappable &operator=(const T val)
    {
        value_ = val;
        return *this;
    };

    [[nodiscard]] auto value() const -> T
    {
        return value_;
    }

    [[nodiscard]] auto map(const std::invocable<T> auto &func) const -> decltype(auto)
    {
        return mappable<decltype(func(this->value()))>(func(this->value()));
    }

    [[nodiscard]] auto map(const auto &object) const -> decltype(auto)
    {
        auto map_fn = [](auto &arg) { return [&arg](T val) { return arg.map(val); }; };

        return map(map_fn(object));
    }

    auto operator<=>(const mappable<T> &) const = default;

private:
    T value_{};
};

struct chars_map
{
    chars_map()
    {
        number_map.reserve(140);
    }
    auto add_line(const std::string &line) -> std::string&
    {
        return number_map.emplace_back(line);
    }
    [[nodiscard]] auto get_at(int64_t x, int64_t y) const -> std::optional<char>
    {
        if (x < 0 || y < 0 || y >= number_map.size())
        {
            return {};
        }
        if (const auto &line = number_map.at(y); x < line.length())
        {
            return line.at(x);
        }
        return {};
    }
    [[nodiscard]] auto set_at(int64_t x, int64_t y, char chr)
    {
        if (x < 0 || y < 0 || y >= number_map.size())
        {
            return;
        }
        if (auto &line = number_map.at(y); x < line.length())
        {
            line.at(x) = chr;
        }
    }
    [[nodiscard]] auto get_line(int y) const -> std::string_view
    {
        if (y < 0 || y >= number_map.size())
        {
            return ""sv;
        }
        return number_map.at(y);
    }
    auto operator[](int x, int y) const -> std::optional<char>
    {
        return get_at(x, y);
    }
    [[nodiscard]] auto lines() -> auto&
    {
        return number_map;
    }
private:
    std::vector<std::string> number_map;
};

} // namespace AoC::common
#endif
