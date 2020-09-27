#include <sorting/radix_sort.hpp>

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "testing_config.hpp"

std::vector<unsigned> generate_vector(std::size_t size, unsigned power_limit)
{
    auto generator = [power_limit]
    {
        static std::mt19937 twister{};
        static std::uniform_int_distribution<unsigned> dist{0u, (1u << power_limit)};
        return dist(twister);
    };

    std::vector<unsigned> v(size);
    std::generate(v.begin(), v.end(), generator);

    return v;
}

#include <atomic>
#include <chrono>

namespace shino
{
template <typename Clock = std::chrono::high_resolution_clock>
class stopwatch
{
    const typename Clock::time_point start_point;

public:
    stopwatch() : start_point(Clock::now())
    {}

    template <typename Rep = typename Clock::duration::rep,
              typename Units = typename Clock::duration>
    Rep elapsed_time() const
    {
        std::atomic_thread_fence(std::memory_order_relaxed);
        auto counted_time = std::chrono::duration_cast<Units>(Clock::now() - start_point).count();
        std::atomic_thread_fence(std::memory_order_relaxed);
        return static_cast<Rep>(counted_time);
    }
};

using precise_stopwatch = stopwatch<>;
using system_stopwatch = stopwatch<std::chrono::system_clock>;
using monotonic_stopwatch = stopwatch<std::chrono::steady_clock>;
} // namespace shino

#include <fstream>
#include <stdexcept>

int main()
{
    std::ofstream measurings("measurings.csv");
    if (!measurings)
        throw std::runtime_error("measurings file opening failed");

    constexpr auto power = 16u;
    //    for (unsigned power = 4; power <= 16; ++power)
    //    {
    for (auto size = 0ull; size <= test_size_max; ++size)
    {
        std::cout << "radix sorting vector of size " << size << '\n';
        auto input = generate_vector(size, power);
        shino::precise_stopwatch stopwatch;
        shino::radix_sort(input.begin(), input.end(), power);
        auto elapsed_time = stopwatch.elapsed_time();
        measurings //<< power << ','
            << size << ',' << elapsed_time << '\n';
        if (not std::is_sorted(input.begin(), input.end()))
            throw std::logic_error("radix sort doesn't work correctly on size " +
                                   std::to_string(size));
    }
    //    }
}