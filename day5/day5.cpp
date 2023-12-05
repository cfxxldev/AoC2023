#include "../AoC.hpp"

namespace day5
{
using namespace AoC::common;

struct mapping_range
{
    int64_t destination_start;
    int64_t source_start;
    int64_t length;

    [[nodiscard]] auto map(int64_t src) const -> int64_t
    {
        if (int64_t distance = src - source_start; distance >= 0 && distance < length)
        {
            return destination_start + distance;
        }
        return -1;
    }

    static auto from_range(std::ranges::range auto rng) -> std::optional<mapping_range>
    {
        if (!rng.empty())
        {
            mapping_range ret{};

            auto it = rng.begin();
            ret.destination_start = *it;
            if (++it != rng.end())
            {
                ret.source_start = *it;
            }
            if (++it != rng.end())
            {
                ret.length = *it;
            }
            return ret;
        }

        return {};
    }

    auto operator<=>(const mapping_range &) const = default;
};

struct ranged_map
{
    auto add_range(mapping_range range)
    {
        return data.insert(range);
    }

    auto add_range(std::optional<mapping_range> range)
    {
        if (range.has_value())
        {
            return std::optional(add_range(range.value()));
        }
        return std::optional<decltype(add_range(range.value()))>{};
    }

    [[nodiscard]] auto map(int64_t src) const -> int64_t
    {
        int64_t dest = src;
        for (const auto range : data)
        {
            if (auto val = range.map(src); val != -1)
            {
                dest = val;
                break;
            }
        }
        return dest;
    }
    auto operator[](int64_t src) const -> int64_t
    {
        return map(src);
    }

private:
    std::set<mapping_range> data;
};

struct seed_almanac
{
    auto parse_line(std::string_view line) -> void
    {
        using namespace std::ranges::views;
        if (line.empty())
        {
            return;
        }
        if (line.starts_with("seeds:"))
        {
            auto seed_ids = line | drop(7) | split(' ') | filter(is_not_empty_fn()) | transform(make_int<int64_t>);
            seeds.insert(seeds.end(), seed_ids.begin(), seed_ids.end());
            current_map = nullptr;
            return;
        }
        current_map = match(line, "seed-to-soil map:"sv, &seed_to_soil).value_or(current_map);
        current_map = match(line, "soil-to-fertilizer map:"sv, &soil_to_fertilizer).value_or(current_map);
        current_map = match(line, "fertilizer-to-water map:"sv, &fertilizer_to_water).value_or(current_map);
        current_map = match(line, "water-to-light map:"sv, &water_to_light).value_or(current_map);
        current_map = match(line, "light-to-temperature map:"sv, &light_to_temperature).value_or(current_map);
        current_map = match(line, "temperature-to-humidity map:"sv, &temperature_to_humidity).value_or(current_map);
        current_map = match(line, "humidity-to-location map:"sv, &humidity_to_location).value_or(current_map);

        if (current_map != nullptr)
        {
            auto mapping = line | split(' ') | transform(make_int<int64_t>);
            current_map->add_range(mapping_range::from_range(mapping));
        }
    }

    auto find_location_part1()
    {
        std::set<int64_t> locations;
        for (auto seed : seeds)
        {
            auto location = humidity_to_location
                [temperature_to_humidity
                     [light_to_temperature
                          [water_to_light[fertilizer_to_water[soil_to_fertilizer[seed_to_soil[seed]]]]]]];
            locations.insert(location);
        }
        return *locations.begin();
    }


    auto find_location_part2_async(int64_t start, int64_t count)
    {
        int64_t location = -1;
        for (int64_t seed_ofs = 0; seed_ofs < count; ++seed_ofs)
        {
            auto loc = humidity_to_location
                [temperature_to_humidity
                     [light_to_temperature
                          [water_to_light[fertilizer_to_water[soil_to_fertilizer[seed_to_soil[start + seed_ofs]]]]]]];
            if (location < 0 || loc < location)
            {
                location = loc;
            }
        }
        return location;
    }

    auto find_location_part2()
    {
        struct chunk
        {
            int64_t start;
            int64_t size;
            int64_t result;
        };
        const int64_t max_chunk_size = 50000000;
        std::vector<chunk> chunks;
        for (auto itSeed = seeds.begin(); itSeed != seeds.end(); itSeed += 2)
        {
            int64_t seed = *itSeed;
            int64_t num_seeds = *(itSeed + 1);
            do
            {
                int64_t chunk_size = std::min(max_chunk_size, num_seeds);
                chunks.emplace_back(seed, chunk_size);
                seed += chunk_size;
                num_seeds -= chunk_size;
            } while (num_seeds > max_chunk_size);
        }
        std::for_each(std::execution::par_unseq, chunks.begin(), chunks.end(), [this](auto &chunk) {
            return chunk.result = find_location_part2_async(chunk.start, chunk.size);
        });
        return std::min_element(std::execution::par_unseq,
                                chunks.begin(),
                                chunks.end(),
                                [](auto &a, auto &b) { return a.result < b.result; })
            ->result;
    }

private:
    ranged_map *current_map = nullptr;
    std::vector<int64_t> seeds;
    ranged_map seed_to_soil;
    ranged_map soil_to_fertilizer;
    ranged_map fertilizer_to_water;
    ranged_map water_to_light;
    ranged_map light_to_temperature;
    ranged_map temperature_to_humidity;
    ranged_map humidity_to_location;
};

} // namespace day5

// g++ --std=c++20 -O3 -ltbb day5.cpp && cat input.txt | ./a.out
auto main() -> int
{
    std::string line;
    int64_t summe_part1 = 0;
    int64_t summe_part2 = 0;
    day5::seed_almanac seed_almanac;
    while (std::getline(std::cin, line))
    {
        seed_almanac.parse_line(line);
    }
    summe_part1 = seed_almanac.find_location_part1();
    summe_part2 = seed_almanac.find_location_part2();
    std::cout << "Sum Part1: " << summe_part1 << '\n';
    std::cout << "Sum Part2: " << summe_part2 << '\n';
    return 0;
}
