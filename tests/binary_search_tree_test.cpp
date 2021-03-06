#include <sorting/binary_search_tree.hpp>

#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_set>

#include "testing_config.hpp"

std::vector<int> generate_unique_numbers(std::size_t size)
{
    std::vector<int> result;
    if (size == 0)
        return {};

    static std::mt19937_64 twister;
    std::uniform_int_distribution<> distribution{0, static_cast<int>(size - 1)};

    std::unordered_set<int> numbers;
    while (numbers.size() != size)
    {
        numbers.insert(distribution(twister));
    }

    return {numbers.begin(), numbers.end()};
}

void run_randomized_insert_tests()
{
    for (std::size_t i = 0; i <= test_size_max; ++i)
    {
        std::cout << "running binary_search_tree insert test on size " << i << '\n';
        auto numbers = generate_unique_numbers(i);
        shino::binary_search_tree<int> tree;
        for (auto x : numbers)
            tree.try_insert(x);

        std::sort(numbers.begin(), numbers.end());
        std::size_t numbers_index = 0;
        for (auto x : tree)
        {
            if (x != numbers[numbers_index++])
                throw std::logic_error{"tree binary_tree_iterator is broken on size " +
                                       std::to_string(i)};
        }
    }
}

void remove_value(std::vector<int>& vec, int x)
{
    vec.erase(std::remove(vec.begin(), vec.end(), x), vec.end());
}

void run_randomized_remove_tests()
{
    static std::mt19937_64 twister;

    for (std::size_t i = 0; i <= test_size_max; ++i)
    {
        shino::binary_search_tree<int> tree;
        auto numbers = generate_unique_numbers(i);
        for (auto x : numbers)
            tree.try_insert(x);
        std::sort(numbers.begin(), numbers.end());
        std::cout << "running remove test on tree of size " << i << '\n';
        for (std::size_t j = 0; j < i; ++j)
        {
            std::bernoulli_distribution dist;
            if (dist(twister))
            {
                tree.delete_if_exists(static_cast<int>(j));
                remove_value(numbers, static_cast<int>(j));
            }
        }
        std::size_t values_index = 0;
        for (auto x : tree)
        {
            if (numbers[values_index] != x)
                throw std::logic_error{"remove doesn't work correctly on " + std::to_string(i)};
            ++values_index;
        }
    }
}

int main()
{
    std::cout << "running randomized insert tests...\n";
    run_randomized_insert_tests();
    std::cout << "randomized insert tests passed successfully\n";

    std::cout << "running randomized remove tests...\n";
    run_randomized_remove_tests();
    std::cout << "randomized remove tests passed successfully\n";
}