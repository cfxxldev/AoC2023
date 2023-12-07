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

} // namespace AoC::common
#endif
