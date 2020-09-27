#include <sorting/quadratic_sorts.hpp>
#include <sorting/quick_sort.hpp>
#include <sorting/utility.hpp>

#include <array>
#include <iterator>
#include <random>
#include <type_traits>

namespace shino
{
template <typename PredefinedGenerator = std::mt19937_64>
class random_selection_strategy
{
    PredefinedGenerator generator;

public:
    template <typename... Arguments>
    explicit random_selection_strategy(Arguments&&... args)
        : generator(std::forward<Arguments>(args)...)
    {}

    template <typename RandomAccessIterator>
    RandomAccessIterator operator()(RandomAccessIterator first, RandomAccessIterator last)
    {
        using difference_type =
            typename std::iterator_traits<RandomAccessIterator>::difference_type;
        std::uniform_int_distribution<difference_type> distribution(0, std::distance(first, last));
        return std::next(first, distribution(generator));
    }
};

inline random_selection_strategy<> default_random_selection;

struct median_of_three
{
    template <typename RandomAccessIterator>
    RandomAccessIterator operator()(RandomAccessIterator first, RandomAccessIterator last)
    {
        auto distance = std::distance(first, last);
        switch (distance)
        {
        case 0:
            return first;
        case 1:
            return first;
        case 2:
            if (*first > *std::prev(last))
                return first;
            else
                return std::prev(last);
        default:;
        }

        auto middle = shino::middle(first, last);
        std::array<RandomAccessIterator, 3> candidates = {first, middle, last};
        shino::insertion_sort(candidates.begin(), candidates.end());
        return candidates[1];
    }
};

template <typename RandomAccessIterator, typename SelectionStrategy = median_of_three>
void strategic_quick_sort(RandomAccessIterator first, RandomAccessIterator last,
                          SelectionStrategy strategy = median_of_three{})
{
    if (first == last)
        return;

    auto partition_point = strategy(first, last);
    std::iter_swap(first, partition_point);
    auto predicate = [first](const auto& elem)
    {
        return elem < *first;
    };
    auto pivot = shino::partition(std::next(first), last, predicate);

    std::iter_swap(std::prev(pivot), first);
    if (std::prev(pivot) != first)
        shino::quick_sort(first, pivot);

    shino::quick_sort(pivot, last);
}
} // namespace shino

#include <algorithm>
#include <iostream>
#include <stdexcept>

void test_random_selection_strategy()
{
    std::mt19937 twister{};
    std::uniform_int_distribution<int> dist{0, 100};
    for (std::size_t i = 0; i <= 30'000; ++i)
    {
        std::vector<int> v(i);
        std::generate(v.begin(), v.end(),
                      [&dist, &twister]
                      {
                          return dist(twister);
                      });

        std::cout << "sorting vector of size " << i << '\n';
        shino::strategic_quick_sort(v.begin(), v.end(), shino::default_random_selection);
        if (not std::is_sorted(v.begin(), v.end()))
            throw std::logic_error("incorrect sorting on size " + std::to_string(i));
    }
}

void test_median_of_three_strategy()
{
    std::mt19937 twister{};
    std::uniform_int_distribution<int> dist{0, 100};
    for (std::size_t i = 0; i <= 30'000; ++i)
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