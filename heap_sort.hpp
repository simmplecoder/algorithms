#ifndef TEMPERATURE_HEAP_SORT_HPP
#define TEMPERATURE_HEAP_SORT_HPP

#include <iterator>
#include <algorithm>
#include <functional>

namespace shino
{
    template <typename RandomAccessIterator, typename Compare = std::less<>>
    void push_heap(RandomAccessIterator first,
                   RandomAccessIterator last,
                   RandomAccessIterator element,
                   Compare cmp = {})
    {
        auto dist = std::distance(first, element) + 1;
        auto left_child = first + dist * 2 - 1;
        if (left_child >= last)
            return;
        auto right_child = first + dist * 2;
        if (right_child >= last)
        {
            if (cmp(*element, *left_child))
            {
                std::iter_swap(element, left_child);
                push_heap(first, last, left_child, cmp);
            }
            return;
        }

        if (not cmp(*element, *left_child) and not cmp (*element, *right_child))
            return;

        auto next_location =
                (not cmp(*left_child,*right_child)) ? left_child : right_child;
        std::iter_swap(next_location, element);
        shino::push_heap(first, last, next_location, cmp);
    }

    template <typename RandomAccessIterator, typename Compare = std::less<>>
    void build_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp = {})
    {
        for (auto middle = first + std::distance(first, last) / 2;
             middle != first; --middle)
        {
            shino::push_heap(first, last, middle, cmp);
        }

        shino::push_heap(first, last, first, cmp);
    }

    template <typename RandomAccessIterator, typename Compare = std::less<>>
    void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp = {})
    {
        if (std::distance(first, last) < 2)
            return;

        auto new_last = std::prev(last);
        std::iter_swap(first, new_last);
        shino::push_heap(first, new_last, first, cmp);
    }

    template <typename RandomAccessIterator, typename Compare = std::less<>>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp = {})
    {
        auto current_last = last;
        while (first != current_last)
        {
            shino::pop_heap(first, current_last, cmp);
            --current_last;
        }
    }
}

#endif //TEMPERATURE_HEAP_SORT_HPP
