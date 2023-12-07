#include "../AoC.hpp"

namespace day7
{
using namespace AoC::common;
enum class cards_enum : char
{
    zero = 0,
    one = 1,
    two = 2,
    three = 3,
    four = 4,
    five = 5,
    six = 6,
    seven = 7,
    eight = 8,
    nine = 9,
    ten = 10,
    J = 11,
    Q = 12,
    K = 13,
    A = 14,
    ERROR = std::numeric_limits<char>::max()
};
[[nodiscard]]auto cards_to_int(const std::array<cards_enum, 5> &card_values) -> uint64_t
{
        uint64_t result{};
        for (auto card : card_values)
        {
            result <<= 8;
            result |= static_cast<char>(card);
        }
        return result;
}
[[nodiscard]] auto get_card_value(char card) -> cards_enum
{
    using namespace matchit;
    return match(card)(
        pattern | 'A' = cards_enum::A,
        pattern | 'K' = cards_enum::K,
        pattern | 'Q' = cards_enum::Q,
        pattern | 'J' = cards_enum::J,
        pattern | 'T' = cards_enum::ten,
        pattern | app(is_digit, true) = [card] { return static_cast<cards_enum>(card - '0'); },
        pattern | _ = [] { throw std::runtime_error("unknown card"); return cards_enum::ERROR; });
}
struct counted_card
{
    long count{};
    cards_enum card{};
    friend auto operator<=>(const counted_card &left, const counted_card &right) = default;
};
struct hand_value
{
    int strength = {};
    uint64_t card_values = {};

    friend auto operator<(const hand_value &left, const hand_value &right)
    {
        return left.strength == right.strength ? left.card_values < right.card_values : left.strength < right.strength;
    }
    friend auto operator<=>(const hand_value &, const hand_value &) = default;
};
struct hand
{
    std::array<cards_enum, 5> cards{};
    int bid{};
    cards_enum best_card{cards_enum::J};

    auto joker_rule()
    {
        using namespace std::ranges::views;
        std::array<cards_enum, 5> tmp{};
        std::transform(cards.cbegin(), cards.cend(), cards.begin(), [](auto arg) { return arg == cards_enum::J ? cards_enum::zero : arg; });

        std::ranges::copy(cards, tmp.begin());
        std::ranges::sort(tmp, std::greater<>());

        auto card_counts = get_card_counts(tmp);
        auto result = card_counts[0].count > 1 ? card_counts[0].card : tmp[0];
        if (result == cards_enum::zero)
        {
            if (card_counts[0].count == 5)
            {
                result = cards_enum::K;
            }
            else
            {
                result = card_counts[1].count > 1 ? card_counts[1].card : tmp[0];
            }
        }
        best_card = result;
    }
    friend auto operator<(const hand &left, const hand &right) -> bool
    {
        auto value1 = left.get_value();
        auto value2 = right.get_value();
        return value1 < value2;
    }
    friend auto operator<=>(const hand &, const hand &) = default;

private:
    [[nodiscard]] auto get_value() const -> hand_value
    {
        hand_value result;
        using namespace std::ranges::views;
        std::array<cards_enum, 5> tmp{};
        result.card_values = cards_to_int(cards);

        std::ranges::copy(cards | transform([this](auto arg) { return arg == cards_enum::zero ? best_card : arg; }),
                          tmp.begin());
        std::ranges::sort(tmp, std::greater<>());

        std::vector<long> card_counts;
        std::ranges::copy(get_card_counts(tmp) | transform([](auto &arg) { return arg.count; }),
                          std::back_inserter(card_counts));

        result.strength = get_hand_strength(card_counts);
        return result;
    }
    [[nodiscard]] static auto get_hand_strength(const std::vector<long> &card_counts)  -> int
    {
        using namespace matchit;
        return match(card_counts)(
            pattern | ds(5) = 100,
            pattern | ds(4, 1) = 90,
            pattern | ds(3, 2) = 80,
            pattern | ds(3, 1, 1) = 70,
            pattern | ds(2, 2, 1) = 60,
            pattern | ds(2, 1, 1, 1) = 50,
            pattern | ds(1, 1, 1, 1, 1) = 0,
            pattern | _ = [] {
                throw std::runtime_error("unknown hand");
                return -1;
            });
    }
    [[nodiscard]] static auto get_card_counts(const std::ranges::range auto &cards) -> std::vector<counted_card>
    {
        using namespace std::ranges::views;
        std::vector<cards_enum> uniques;
        std::ranges::unique_copy(cards, std::back_inserter(uniques));

        std::vector<counted_card> card_counts;
        std::ranges::copy(uniques | transform([&cards](auto arg) {
                            auto count = std::ranges::count_if(cards, [arg](auto arg1) { return arg == arg1; });
                            return counted_card{count, arg};
                        }),
                        std::back_inserter(card_counts));
        std::ranges::sort(card_counts, std::greater<>());
        return card_counts;
    }
};
struct card_game
{
    auto parse_line(std::string_view line) -> void
    {
        auto &hand = hands.emplace_back();
        std::ranges::copy(line.substr(0, 5) | std::views::transform(get_card_value), hand.cards.begin());
        hand.bid = to_int(line.substr(6));
    }
    [[nodiscard]] auto get_part1() const -> auto
    {
        auto temp = hands;
        std::ranges::stable_sort(temp);

        int64_t summe = 0;
        for (int64_t idx = 0; idx < temp.size(); ++idx)
        {
            const auto &hand = temp.at(idx);
            summe += static_cast<int64_t>(hand.bid) * (idx + 1);
        }
        return summe;
    }
    [[nodiscard]] auto get_part2() const -> auto
    {
        std::vector<hand> temp = hands;
        std::for_each(temp.begin(), temp.end(), [](auto &arg) { arg.joker_rule(); });
        std::ranges::stable_sort(temp);

        int64_t summe = 0;
        for (int64_t idx = 0; idx < temp.size(); ++idx)
        {
            auto &hand = temp.at(idx);
            summe += static_cast<int64_t>(hand.bid) * (idx + 1);
        }
        return summe;
    }
private:
    std::vector<hand> hands;
};
} // namespace day7

// g++ --std=c++20 -O3 -ltbb day7.cpp && cat input.txt | ./a.out
auto main() -> int
{
    std::string line;
    int64_t summe_part1 = 0;
    int64_t summe_part2 = 0;
    day7::card_game card_game;
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
