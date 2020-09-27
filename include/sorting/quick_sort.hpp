#ifndef TEMPERATURE_QUICK_SORT_HPP
#define TEMPERATURE_QUICK_SORT_HPP

#include <iterator>
#include <algorithm>
#include <random>

namespace shino
{
    template <typename RandomAccessIterator,
              typename Predicate>
    RandomAccessIterator partition(RandomAccessIterator first,
                                   RandomAccessIterator last,
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

        auto predicate = [&first](const auto& elem) { return elem < *first; };
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
        auto predicate = [first](const auto& elem) { return elem < *first; };

        auto pivot = shino::partition(std::next(first), last, predicate);
        std::iter_swap(std::prev(pivot), first);
        if (std::prev(pivot) != first)
            shino::randomized_quicksort(first, pivot);

        shino::randomized_quicksort(pivot, last);
    }
}

#endif //TEMPERATURE_QUICK_SORT_HPP
