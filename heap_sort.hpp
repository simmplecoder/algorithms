#ifndef TEMPERATURE_HEAP_SORT_HPP
#define TEMPERATURE_HEAP_SORT_HPP

#include <iterator>
#include <algorithm>

namespace shino
{
    template <typename RandomAccessIterator>
    void push_heap(RandomAccessIterator first,
                   RandomAccessIterator last,
                   RandomAccessIterator element)
    {
        auto dist = std::distance(first, element) + 1;
        auto left_child = first + dist * 2 - 1;
        if (left_child >= last)
            return;
        auto right_child = first + dist * 2;
        if (right_child >= last)
        {
            if (*element < *left_child)
            {
                std::iter_swap(element, left_child);
                push_heap(first, last, left_child);
            }
            return;
        }

        if (*element >= *left_child and *element >= *right_child)
            return;

        auto next_location =
                (*left_child >= *right_child) ? left_child : right_child;
        std::iter_swap(next_location, element);
        shino::push_heap(first, last, next_location);
    }

    template <typename RandomAccessIterator>
    void build_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        for (auto middle = first + std::distance(first, last) / 2;
             middle != first; --middle)
        {
            shino::push_heap(first, last, middle);
        }

        shino::push_heap(first, last, first);
    }

    template <typename RandomAccessIterator>
    void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        if (std::distance(first, last) < 2)
            return;

        auto new_last = std::prev(last);
        std::iter_swap(first, new_last);
        shino::push_heap(first, new_last, first);
    }

    template <typename RandomAccessIterator>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        auto current_last = last;
        while (first != current_last)
        {
            shino::pop_heap(first, current_last);
            --current_last;
        }
    }
}

#endif //TEMPERATURE_HEAP_SORT_HPP
