#include "quick_sort.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <unordered_set>

std::vector<int> generate_vector(std::size_t size)
{
    static std::mt19937 twister{};
    static std::uniform_int_distribution<int> dist{0, 9};

    std::unordered_set<int> numbers;
    while (numbers.size() < size)
        numbers.insert(dist(twister));

    return {numbers.begin(), numbers.end()};
}

std::vector<int> v{generate_vector(6)};

#include <iterator>
#include <algorithm>

int main()
{
    std::mt19937 twister{};
    std::uniform_int_distribution<int> dist{};
    for (std::size_t i = 0; i <= 30'000; ++i)
    {
        std::vector<int> v(i);
        std::generate(v.begin(), v.end(), [&dist, &twister]{return dist(twister);});

        std::cout << "sorting vector of size " << i << '\n';
        quick_sort(v.begin(), v.end());
        if (not std::is_sorted(v.begin(), v.end()))
        {
            std::clog << "incorrect sorting on size " << i << '\n';
        }
    }
}