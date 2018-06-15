#ifndef ALGORITHMS_MERGE_SORT_HPP
#define ALGORITHMS_MERGE_SORT_HPP

#include <iterator>
#include <algorithm>
#include <vector>

namespace shino
{
    template <typename Iterator>
    constexpr Iterator middle(Iterator first, Iterator last)
    {
        auto distance = std::distance(first, last);
        return first + distance / 2;
    }

    template <typename Container>
    constexpr typename Container::iterator middle(Container& container)
    {
        return middle(container.begin(), container.end());
    }

    template <typename Iterator, typename OutputIterator>
    void merge(Iterator first, Iterator middle, Iterator last, OutputIterator d_first)
    {
        auto first2 = middle;
        while (first != middle and first2 != last)
        {
            if (*first < *first2)
                *d_first++ = *first++;
            else
                *d_first++ = *first2++;
        }

        if (first != middle)
            std::copy(first, middle, d_first);
        else
            std::copy(first2, last, d_first);
    }

    namespace detail
    {
        template <typename Iterator>
        using buffer_iterator = typename std::vector<typename std::iterator_traits<Iterator>::value_type>::iterator;

        template <typename Iterator>
        void merge_sort(Iterator first, Iterator last,
                        buffer_iterator<Iterator> buffer_first)
        {
            if (std::distance(first, last) < 2)
                return;

            auto distance = std::distance(first, last);
            auto half_distance = distance / 2;
            auto middle_iterator = first + half_distance;
            merge_sort(first, middle_iterator, buffer_first);
            merge_sort(middle_iterator, last, buffer_first + half_distance);

            merge(first, middle_iterator, last, buffer_first);
            std::copy(buffer_first, buffer_first + distance, first);
        }
    }

    template <typename Iterator>
    void merge_sort(Iterator first, Iterator last)
    {
        if (std::distance(first, last) < 2)
            return;

        using value_type = typename std::iterator_traits<Iterator>::value_type;
        auto distance = std::distance(first, last);
        std::vector<value_type> buffer(distance);
        detail::merge_sort(first, last, buffer.begin());
    }
}

#endif //ALGORITHMS_MERGE_SORT_HPP
