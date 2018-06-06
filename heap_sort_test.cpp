//#include "heap_sort.hpp"
#include <vector>
#include <iostream>
#include <random>
#include <unordered_set>
#include <numeric>
#include <algorithm>

std::vector<int> generate_vector(std::size_t size)
{
    if (size == 0)
        return {};
    if (size == 1)
        return {0};

    static std::mt19937 twister{};
    std::vector<int> numbers(size);
    std::iota(numbers.begin(), numbers.end(), 0);
    std::shuffle(numbers.begin(), numbers.end(), twister);

    return numbers;
}

std::vector<int> global_v{generate_vector(4)};

#include <iterator>

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

#include <algorithm>

int main()
{
//    shino::build_heap(global_v.begin(), global_v.end());
//    if (not std::is_heap(global_v.begin(), global_v.end()))
//        std::cerr << "not a heap\n";
//    shino::sort_heap(global_v.begin(), global_v.end());
//
//    for (int x: global_v)
//        std::cout << x << ' ';
//    std::cout << '\n';
    for (std::size_t i = 0; i <= 10'000; ++i)
    {
        std::vector<int> v(generate_vector(i));

        std::cout << "heapifying vector of size " << i << '\n';
        shino::build_heap(v.begin(), v.end());
        if (not std::is_heap(v.begin(), v.end()))
            std::cerr << "incorrect heapifying on size " << i << '\n';

        shino::sort_heap(v.begin(), v.end());
        if (not std::is_sorted(v.begin(), v.end()))
            std::cerr << "incorrect heap sorting on size " << i << '\n';
    }
}

