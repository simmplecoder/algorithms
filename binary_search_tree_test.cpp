#include "binary_search_tree.hpp"

#include <random>
#include <unordered_set>
#include <algorithm>
#include <iostream>


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
    for (std::size_t i = 0; i <= 5'000; ++i)
    {
        std::cout << "running binary_search_tree insert test on size " << i << '\n';
        auto numbers = generate_unique_numbers(i);
        shino::binary_search_tree<int> tree;
        for (auto x: numbers)
            tree.try_insert(x);

        std::sort(numbers.begin(), numbers.end());
        std::size_t numbers_index = 0;
        for (auto x: tree)
        {
            if (x != numbers[numbers_index++])
                throw std::logic_error{"tree binary_tree_iterator is broken on size " + std::to_string(i)};
        }
    }
}

void remove_value(std::vector<int>& vec, int x)
{
//    for (auto iter = v.begin(); iter != v.end(); ++iter)
//        if (*iter == x)
//            v.erase(iter);
    vec.erase(std::remove(vec.begin(), vec.end(), x), vec.end());
}

void run_randomized_remove_tests()
{
    static std::mt19937_64 twister;

    for (std::size_t i = 0; i <= 1'000; ++i)
    {
        shino::binary_search_tree<int> tree;
        auto numbers = generate_unique_numbers(i);
        for (auto x: numbers)
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
        for (auto x: tree)
        {
            if (numbers[values_index] != x)
                throw std::logic_error{"remove doesn't work correctly on " + std::to_string(i)};
            ++values_index;
        }
    }
}

int main(){
    std::cout << "running randomized insert tests...\n";
    run_randomized_insert_tests();
    std::cout << "randomized insert tests passed successfully\n";

    std::cout << "running randomized remove tests...\n";
    run_randomized_remove_tests();
    std::cout << "randomized remove tests passed successfully\n";
//    shino::binary_search_tree<int> tree;
//    std::vector<int> numbers{5, 4, 9, 8, 7, 0, 2, 1, 6, 3, 10};
//    for (auto x: numbers)
//        tree.try_insert(x);
//
//    remove_value(numbers, 3);
//    tree.delete_if_exists(3);
//
//    remove_value(numbers, 4);
//    tree.delete_if_exists(4);
}