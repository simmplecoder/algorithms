#ifndef TEMPERATURE_QUICK_SORT_HPP
#define TEMPERATURE_QUICK_SORT_HPP

#include <iterator>
#include <algorithm>

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

    auto predicate = [&first](const auto& elem)
    {return elem < *first;};
    auto pivot = ::partition(std::next(first), last, predicate);

    std::swap(*std::prev(pivot), *first);
    if (std::prev(pivot) != first)
        quick_sort(first, pivot);

    quick_sort(pivot, last);
}

#endif //TEMPERATURE_QUICK_SORT_HPP
