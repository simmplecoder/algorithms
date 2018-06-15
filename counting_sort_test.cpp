#include <utility>
#include <type_traits>
#include <iterator>
#include <vector>

namespace shino
{
    template <typename Iterator,
              typename ValueType = typename std::iterator_traits<Iterator>::value_type,
              typename = std::enable_if_t<std::is_integral_v<ValueType>, void>>
    void counting_sort(Iterator first, Iterator last,
                       ValueType upper_bound, ValueType lower_bound = 0)
    {
        //double minus as lower_bound is negative
        std::vector<std::size_t> occurence_counters(upper_bound - lower_bound + 1);
        auto start = first;
        while (first != last)
            ++occurence_counters[-lower_bound + *first++]; //lower_bound is minus, thus double minus

        auto current = start;
        for (ValueType i = 0; i <= upper_bound - lower_bound; ++i)
        {
            auto next = std::next(current, occurence_counters[i]);
            std::fill(current, next, i + lower_bound);
            current = next;
        }
    }
}

#include <algorithm>
#include <random>
#include <iostream>

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
    for (std::size_t i = 0; i <= 20'000; ++i)
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
    for (std::size_t i = 0; i <= 20'000; ++i)
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