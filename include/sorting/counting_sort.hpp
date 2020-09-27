#ifndef ALGORITHMS_COUNTING_SORT_HPP
#define ALGORITHMS_COUNTING_SORT_HPP

#include <utility>
#include <type_traits>
#include <iterator>
#include <vector>

namespace shino
{
    template <typename ForwardIterator,
              typename ValueType = typename std::iterator_traits<ForwardIterator>::value_type>
    void counting_sort(ForwardIterator first, ForwardIterator last,
                       ValueType upper_bound, ValueType lower_bound = 0)
    {
        static_assert(std::is_integral_v<ValueType>);

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

#endif //ALGORITHMS_COUNTING_SORT_HPP
