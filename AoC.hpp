#ifndef _AoC_included_
#define _AoC_included_

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <cstdint>
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

auto match(auto arg1, auto arg2, auto ret) -> std::optional<decltype(ret)>
{
    if (arg1 == arg2)
    {
        return ret;
    }
    return {};
}
} // namespace AoC::common
#endif
