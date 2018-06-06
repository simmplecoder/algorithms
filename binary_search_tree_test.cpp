#include "binary_search_tree.hpp"

#include <iostream>
#include <sstream>

void test_remove_case_one()
{
    shino::binary_search_tree<int> tree;
    tree.try_insert(2);
    tree.try_insert(3);
    tree.try_insert(1);
    tree.try_insert(4);
    tree.try_insert(-2);
    tree.try_insert(0);

    tree.delete_if_exists(3);
    std::ostringstream oss;
    tree.inorder_print(oss);
    std::string output = oss.str();
    if (output != "-2 0 1 2 4 ")
        throw std::logic_error("remove case one fails");
}

void test_remove_case_two()
{
    shino::binary_search_tree<int> tree;
    tree.try_insert(4);
    tree.try_insert(7);
    tree.try_insert(11);
    tree.try_insert(1);
    tree.try_insert(-2);
    tree.try_insert(0);

    tree.delete_if_exists(4);
    std::ostringstream oss;
    tree.inorder_print(oss);
    auto output = oss.str();
    if (output != "-2 0 1 7 11 ")
        throw std::logic_error("remove case two fails");
}

//almost like case 2, but has three added in it
void test_remove_case_three()
{
    shino::binary_search_tree<int> tree;
    tree.try_insert(4);
    tree.try_insert(7);
    tree.try_insert(11);
    tree.try_insert(1);
    tree.try_insert(-2);
    tree.try_insert(0);
    tree.try_insert(3);

    tree.delete_if_exists(4);
    std::ostringstream oss;
    tree.inorder_print(oss);
    auto output = oss.str();
    if (output != "-2 0 1 3 7 11 ")
        throw std::logic_error("remove case two fails");
}

void print_tree()
{
    shino::binary_search_tree<int> tree;
    tree.try_insert(4);
    tree.try_insert(7);
    tree.try_insert(11);
    tree.try_insert(1);
    tree.try_insert(-2);
    tree.try_insert(0);
    tree.try_insert(3);
    for (auto x: tree)
    {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

#include <random>
#include <unordered_set>
#include <algorithm>

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

void run_randomized_tests()
{
    for (std::size_t i = 0; i <= 10'000; ++i)
    {
        std::cout << "running binary_search_tree test on size " << i << '\n';
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

int main(){
    std::cout << "running remove case 1...\n";
    test_remove_case_one();
    std::cout << "remove case 1 passed successfully\n";
    std::cout << "running remove case 2...\n";
    test_remove_case_two();
    std::cout << "remove case 2 passed successfully\n";
    std::cout << "running remove case 3...\n";
    test_remove_case_three();
    std::cout << "remove case 3 passed successfully\n";
//
    std::cout << "running randomized tests...\n";
    run_randomized_tests();
    std::cout << "randomized tests passed successfully\n";
    shino::binary_search_tree<int> tree;
}