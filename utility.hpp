#ifndef ALGORITHMS_UTILITY_HPP
#define ALGORITHMS_UTILITY_HPP

#include <iterator>

namespace shino {
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
}

#endif //ALGORITHMS_UTILITY_HPP
