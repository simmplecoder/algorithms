#ifndef ALGORITHMS_RADIX_SORT_HPP
#define ALGORITHMS_RADIX_SORT_HPP

#include <iterator>
#include <algorithm>

namespace shino
{
    template <typename BidirIterator>
    void radix_sort(BidirIterator first, BidirIterator last, unsigned upper_power_bound)
    {
        using category = typename std::iterator_traits<BidirIterator>::iterator_category;
        using value_type = typename std::iterator_traits<BidirIterator>::value_type;
        static_assert(std::is_base_of_v<std::bidirectional_iterator_tag, category>);
        static_assert(std::is_integral_v<value_type>);
        static_assert(std::is_unsigned_v<value_type>);

        value_type mask = 1;
        for (auto power = 0u; power <= upper_power_bound; ++power)
        {
            std::stable_partition(first, last, [mask](auto v) { return !(v & mask); });
            mask <<= 1;
        }
    }
}

#endif //ALGORITHMS_RADIX_SORT_HPP
