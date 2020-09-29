#ifndef ALGORITHMS_QUADRATIC_SORTS_HPP
#define ALGORITHMS_QUADRATIC_SORTS_HPP

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <utility>

namespace shino
{
template <typename RandomAccessIt, typename Compare = std::less<>>
void bubble_sort(RandomAccessIt first, RandomAccessIt last, Compare cmp = {})
{
    if (std::distance(first, last) < 2)
        return;

    for (auto i = std::prev(last); i != first; --i)
    {
        bool sorted = true;
        for (auto j = first; j != i; ++j)
            if (not cmp(*j, *std::next(j)))
            {
                std::iter_swap(j, std::next(j));
                sorted = false;
            }

        if (sorted)
            break;
    }
}

template <typename RandomAccessIt, typename Compare = std::less<>>
void insertion_sort(RandomAccessIt first, RandomAccessIt last, Compare cmp = {})
{
    if (std::distance(first, last) < 2)
        return;

    for (auto i = std::next(first); i != last; ++i)
    {
        auto j = i;
        while (std::prev(j) != first && not cmp(*std::prev(j), *j))
        {
            std::iter_swap(std::prev(j), j);
            --j;
        }
    }
}

template <typename RandomAccessIt, typename Compare = std::less<>>
void selection_sort(RandomAccessIt first, RandomAccessIt last, Compare cmp = {})
{
    if (std::distance(first, last) < 2)
        return;

    for (auto i = first; i != last; ++i)
        for (auto j = std::next(i); j != last; ++j)
            if (not cmp(*i, *j))
                std::iter_swap(i, j);
}

template <typename RandomAccessIt, typename Compare = std::less<>>
void shaker_sort(RandomAccessIt first, RandomAccessIt last, Compare cmp = {})
{
    if (std::distance(first, last) < 2)
        return;

    for (auto i = std::prev(last); i != first; --i)
    {
        bool sorted = true;
        for (auto j = first; j != i; ++j)
            if (not cmp(*j, *std::next(j)))
            {
                std::iter_swap(j, std::next(j));
                sorted = false;
            }

        if (sorted)
            break;

        sorted = true;
        // i is already dereferencable
        for (auto j = i; j != first; --j)
            if (not cmp(*std::prev(j), *j))
            {
                std::iter_swap(std::prev(j), j);
                sorted = false;
            }

        if (sorted)
            break;
    }
}

template <typename RandomAccessIt, typename Compare = std::less_equal<>>
void comb_sort(RandomAccessIt first, RandomAccessIt last, double shrink = 1.3, Compare cmp = {})
{
    auto dist = std::distance(first, last);
    if (dist < 2)
        return;

    bool sorted = false;
    while (not sorted)
    {
        auto j0 = first;
        auto gap = std::floor(dist / shrink);
        shrink *= shrink;
        if (gap > 1)
            sorted = false;
        else
        {
            gap = 1;
            sorted = true;
        }
        auto j1 = std::next(j0, gap);

        for (; j1 < last; ++j0, ++j1)
            if (not cmp(*j0, *j1))
            {
                std::iter_swap(j0, j1);
                sorted = false;
            }
    }
}
} // namespace shino

#endif // ALGORITHMS_QUADRATIC_SORTS_HPP
