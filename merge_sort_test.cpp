#include "merge_sort.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

std::vector<int> generate_vector(std::size_t size)
{
    auto generator = []
    {
        static std::mt19937 twister{};
        static std::uniform_int_distribution<int> dist;
        return dist(twister);
    };

    std::vector<int> v(size);
    std::generate(v.begin(), v.end(), generator);

    return v;
}

std::vector<int> vector_for_merge(std::size_t size)
{
    auto generator = []
    {
        static std::mt19937 twister{};
        static std::uniform_int_distribution<int> dist;
        return dist(twister);
    };

    std::vector<int> v(size);
    std::generate(v.begin(), v.end(), generator);
    std::sort(v.begin(), v.begin() + size / 2);
    std::sort(v.begin() + size / 2, v.end());
    return v;
}

void merge_test()
{
    for (std::size_t i = 0; i <= 20'000; ++i)
    {
        auto v = vector_for_merge(i);
        decltype(v) v3(v.size());
        std::cout << "running merge test on size " << i << '\n';
        shino::merge(v.begin(), shino::middle(v), v.end(), v3.begin());
        if (not std::is_sorted(v3.begin(), v3.end()))
            throw std::logic_error("merge doesn't work for size " + std::to_string(i));
    }
}

void merge_sort_test()
{
    for (std::size_t i = 0; i <= 20'000; ++i)
    {
        auto v = generate_vector(i);
        std::cout << "running merge sort test on size " << i << '\n';
        shino::merge_sort(v.begin(), v.end());
        if (not std::is_sorted(v.begin(), v.end()))
            throw std::logic_error("merge sort doesn't work for size " + std::to_string(i));
    }
}

int main()
{
    merge_test();
    merge_sort_test();
}