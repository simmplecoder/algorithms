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

        enum class direction
        {
            is_root,
            left,
            right
        };

        struct search_result
        {
            //actually used in structured binding,
            // but clion doesn't recognize that
            node* parent;
            node* target_child;
            direction parent_to_child;
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
            bool is_inserted = try_insert_helper(value, root);
            element_count += is_inserted;
            return is_inserted;
        }

        bool exists(const ValueType& value)
        {
            return find_node(value, nullptr, root, direction::is_root).target_child != nullptr;
        }

        bool delete_if_exists(const ValueType& value)
        {
            if (element_count == 0)
                return false;

            auto child_ptr = find_extraction_location(value);
            if (*child_ptr == nullptr)
                return false;

            *child_ptr = find_replacement(*child_ptr);
            --element_count;
            return true;
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
            if (start_pos->left == nullptr)
            {
                auto replacement = start_pos->right;
                delete start_pos;
                return replacement;
            }

            auto descendant = start_pos->left;
            while (descendant->right != nullptr)
                descendant = descendant->right;

            descendant->right = start_pos->right;
            delete start_pos;
            return start_pos->left;
        }

        node** find_extraction_location(const ValueType& value)
        {
            auto* current = &root;
            while (*current != nullptr and (*current)->value != value)
                if (value < (*current)->value)
                    current = &(*current)->left;
                else
                    current = &(*current)->right;

            return {current};
        }

        void clear_helper(node* start_position)
        {
            if (start_position == nullptr)
                return;
            clear_helper(start_position->left);
            clear_helper(start_position->right);

            delete start_position;
        }

        search_result find_node(const ValueType& value,
                                node* parent,
                                node* current_node,
                                direction parent_to_child)
        {
            if (current_node == nullptr)
                return {nullptr, nullptr, direction::is_root};

            if (current_node->value == value)
                return {parent, current_node, parent_to_child};

            if (value < current_node->value)
                return find_node(value, current_node, current_node->left, direction::left);
            else
                return find_node(value, current_node, current_node->right, direction::right);
        }

        bool exists_helper(const ValueType& value,
                           node* current_node)
        {
            if (current_node == nullptr)
                return false;
            if (current_node->value == value)
                return true;

            if (value < current_node->value)
                return exists_helper(value, current_node->left);
            else
                return exists_helper(value, current_node->right);
        }

        bool try_insert_helper(const ValueType& value,
                               node*& current_node)
        {
            if (current_node == nullptr)
            {
                current_node = new node{value};
                return true;
            }

            if (current_node->value == value)
                return false;

            if (current_node->value > value)
                return try_insert_helper(value, current_node->left);
            else
                return try_insert_helper(value, current_node->right);
        }
    };
}

#endif //ALGORITHMS_BINARY_SEARCH_TREE_HPP
