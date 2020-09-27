#ifndef TEMPERATURE_QUICK_SORT_HPP
#define TEMPERATURE_QUICK_SORT_HPP

#include <sorting/quadratic_sorts.hpp>
#include <sorting/utility.hpp>

#include <algorithm>
#include <iterator>
#include <random>

namespace shino
{
template <typename RandomAccessIterator, typename Predicate>
RandomAccessIterator partition(RandomAccessIterator first, RandomAccessIterator last,
                               Predicate predicate)
{
    auto current = first;
    while (current != last and predicate(*current))
        ++current;

    if (current == last)
        return last;

    auto prev_bad_pos = current++;
    while (current != last)
    {
        if (predicate(*current))
        {
            std::iter_swap(current, prev_bad_pos);
            ++prev_bad_pos;
        }
        ++current;
    }

    return prev_bad_pos;
}

template <typename RandomAccessIterator>
void quick_sort(RandomAccessIterator first, RandomAccessIterator last)
{
    if (first == last)
        return;

    auto predicate = [&first](const auto& elem)
    {
        return elem < *first;
    };
    auto pivot = shino::partition(std::next(first), last, predicate);

    std::iter_swap(std::prev(pivot), first);
    if (std::prev(pivot) != first)
        shino::quick_sort(first, pivot);

    shino::quick_sort(pivot, last);
}

template <typename RandomAccessIterator>
void randomized_quicksort(RandomAccessIterator first, RandomAccessIterator last)
{
    auto distance = std::distance(first, last);
    if (distance < 2)
        return;

    static std::mt19937_64 twister{};

    std::uniform_int_distribution<std::size_t> distribution(0, static_cast<size_t>(distance) - 1);

    auto target_elem = std::next(first, distribution(twister));
    std::iter_swap(first, target_elem);
    auto predicate = [first](const auto& elem)
    {
        return elem < *first;
    };

    auto pivot = shino::partition(std::next(first), last, predicate);
    std::iter_swap(std::prev(pivot), first);
    if (std::prev(pivot) != first)
        shino::randomized_quicksort(first, pivot);

    shino::randomized_quicksort(pivot, last);
}

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

#endif // TEMPERATURE_QUICK_SORT_HPP
