#include <sorting/quadratic_sorts.hpp>

#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

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

int main()
{
    for (std::size_t i = 0; i <= 4000; ++i)
    {
        auto v = generate_vector(i);
        std::cout << "Sorting vector with size " << i << '\n';
        shino::comb_sort(v.begin(), v.end(), 1.3);
        if (!std::is_sorted(v.begin(), v.end()))
        {
            throw std::logic_error("comb sort doesn't work on vector size " + std::to_string(i));
        }
    }
}