#include <sorting/counting_sort.hpp>

#include <algorithm>
#include <iostream>
#include <random>

std::vector<int> generate_vector(std::size_t size)
{
    auto generator = []
    {
        static std::mt19937 twister{};
        static std::uniform_int_distribution<int> dist{0, 100};
        return dist(twister);
    };

    std::vector<int> v(size);
    std::generate(v.begin(), v.end(), generator);

    return v;
}

std::vector<int> generate_signed_integers(std::size_t size)
{
    auto generator = []
    {
        static std::mt19937 twister{};
        static std::uniform_int_distribution<int> dist{-50, 50};
        return dist(twister);
    };

    std::vector<int> v(size);
    std::generate(v.begin(), v.end(), generator);

    return v;
}

void test_unsigned_count_sort()
{
    for (std::size_t i = 0; i <= 30'000; ++i)
    {
        std::cout << "count sorting unsigned integer vector of size " << i << '\n';
        auto v = generate_vector(i);
        shino::counting_sort(v.begin(), v.end(), 100);
        if (not std::is_sorted(v.begin(), v.end()))
            throw std::logic_error("count sort doesn't work on size " + std::to_string(i));
    }
}

void test_signed_count_sort()
{
    for (std::size_t i = 0; i <= 30'000; ++i)
    {
        std::cout << "count sorting signed integer vector of size " << i << '\n';
        auto v = generate_signed_integers(i);
        shino::counting_sort(v.begin(), v.end(), 50, -50);
        if (not std::is_sorted(v.begin(), v.end()))
            throw std::logic_error("count sort doesn't work on size " + std::to_string(i));
    }
}

int main()
{
    test_unsigned_count_sort();
    test_signed_count_sort();
}