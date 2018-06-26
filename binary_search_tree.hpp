#ifndef ALGORITHMS_BINARY_SEARCH_TREE_HPP
#define ALGORITHMS_BINARY_SEARCH_TREE_HPP

#include "binary_tree_iterator.hpp"
#include <ostream>
#include <utility>

namespace shino
{
    template <typename ValueType>
    class binary_search_tree
    {
        struct node
        {
            const ValueType value;
            node* left;
            node* right;
        };

        node* root;
        std::size_t element_count;
    public:
        using iterator = binary_tree_iterator<node>;

        binary_search_tree() :
                root(nullptr),
                element_count(0)
        {}

        binary_search_tree(const binary_search_tree& other) = delete;

        binary_search_tree& operator=(const binary_search_tree& other) = delete;

        binary_search_tree(binary_search_tree&& other) noexcept:
                root(std::exchange(other.root, nullptr)),
                element_count(std::exchange(other.element_count, 0))
        {}

        binary_search_tree& operator=(binary_search_tree&& other) noexcept
        {
            std::swap(root, other.root);
            std::swap(element_count, other.element_count);
            return *this;
        }

        bool try_insert(const ValueType& value)
        {
            auto insertion_point = find_node(value);
            if (*insertion_point)
                return false;
            *insertion_point = new node{value};
            ++element_count;
            return true;
        }

        bool exists(const ValueType& value)
        {
            return *find_node(value) != nullptr;
        }

        bool delete_if_exists(const ValueType& value)
        {
            if (element_count == 0)
                return false;

            auto child_ptr = find_node(value);
            if (!*child_ptr)
                return false;

            *child_ptr = find_replacement(*child_ptr);
            --element_count;
            return true;
        }

        std::size_t size() {
            return element_count;
        }

        void clear()
        {
            clear_helper(root);
        }
        
        iterator begin()
        {
            return iterator{root};
        }

        iterator end()
        {
            return {};
        }

        ~binary_search_tree()
        {
            clear();
        }

    private:
        node* find_replacement(node* start_pos)
        {
            if (!start_pos->left)
            {
                auto replacement = start_pos->right;
                delete start_pos;
                return replacement;
            }

            auto descendant = start_pos->left;
            while (descendant->right)
                descendant = descendant->right;

            descendant->right = start_pos->right;
            delete start_pos;
            return start_pos->left;
        }

        void clear_helper(node* start_position)
        {
            if (!start_position)
                return;
            clear_helper(start_position->left);
            clear_helper(start_position->right);

            delete start_position;
        }

        node** find_node(const ValueType& value)
        {
            auto* current = &root;
            while (*current and (*current)->value != value)
                if (value < (*current)->value)
                    current = &(*current)->left;
                else
                    current = &(*current)->right;

            return current;
        }
    };
}

#endif //ALGORITHMS_BINARY_SEARCH_TREE_HPP
