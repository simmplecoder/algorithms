#include <sorting/quick_sort.hpp>

#include <array>
#include <iterator>
#include <random>
#include <type_traits>

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include "testing_config.hpp"

void test_random_selection_strategy()
{
    std::mt19937 twister{};
    std::uniform_int_distribution<int> dist{0, 100};
    for (std::size_t i = 0; i <= test_size_max; ++i)
    {
        std::vector<int> v(i);
        std::generate(v.begin(), v.end(),
                      [&dist, &twister]
                      {
                          return dist(twister);
                      });

        std::cout << "sorting vector of size " << i << '\n';
        shino::strategic_quick_sort(v.begin(), v.end(), shino::random_selection_strategy<>{});
        if (not std::is_sorted(v.begin(), v.end()))
            throw std::logic_error("incorrect sorting on size " + std::to_string(i));
    }
}

void test_median_of_three_strategy()
{
    std::mt19937 twister{};
    std::uniform_int_distribution<int> dist{0, 100};
    for (std::size_t i = 0; i <= test_size_max; ++i)
    {
        std::vector<int> v(i);
        std::generate(v.begin(), v.end(),
                      [&dist, &twister]
                      {
                          return dist(twister);
                      });

        std::cout << "sorting vector of size " << i << '\n';
        shino::strategic_quick_sort(v.begin(), v.end());
        if (not std::is_sorted(v.begin(), v.end()))
            throw std::logic_error("incorrect sorting on size " + std::to_string(i));
    }
}

int main()
{
    test_random_selection_strategy();
    test_median_of_three_strategy();
}