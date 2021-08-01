#include "sorting/heap_sort.hpp"
#include <vector>
#include <algorithm>

int main() {
    std::vector v{4, 1, 3, 2};
    shino::sort_heap(v.begin(), v.end());
    return !std::is_sorted(v.begin(), v.end());
}
