#ifndef ALGORITHMS_BINARY_SEARCH_TREE_HPP
#define ALGORITHMS_BINARY_SEARCH_TREE_HPP

#include "binary_tree_iterator.hpp"
#include <ostream>
#include <utility>
#include <type_traits>
#include <memory>

namespace shino
{
    template <typename ValueType>
    class binary_search_tree
    {
        constexpr static bool v_copiable = std::is_copy_constructible_v<ValueType>;
        constexpr static bool v_moveable = std::is_move_constructible_v<ValueType>;
        struct node
        { //DO NOT MOVE ELEMENTS AROUND, emplace relies on this order
            const ValueType value;
            node* left = nullptr;
            node* right = nullptr;
        };

        node* root = nullptr;
        std::size_t element_count = 0;
    public:
        using iterator = binary_tree_iterator<node>;

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

        template <typename = std::enable_if_t<v_copiable>>
        explicit binary_search_tree(const binary_search_tree& other)
        {
            if (other.element_count == 0)
                return;
            root = new node(other.root->value);
            deep_copy(root->left, other.root->left);
            deep_copy(root->right, other.root->right);

            element_count = other.element_count;
        }

        template <typename AnotherType,
                  typename = std::enable_if_t<std::is_same_v<ValueType, std::decay_t<AnotherType>>
                                                     and (v_copiable || v_moveable)>>
        bool try_insert(AnotherType&& value)
        {
            auto insertion_point = find_node(value);
            if (*insertion_point)
                return false;
            *insertion_point = new node{std::forward<AnotherType>(value)};
            ++element_count;
            return true;
        }

        template <typename ... Args>
        bool emplace(Args&& ... args)
        {
            std::unique_ptr<char[]> buffer = std::make_unique<char[]>(sizeof(node));
            new (buffer.get()) node(std::forward<Args>(args)...);
            auto possible_node = reinterpret_cast<node*>(buffer.get());
            auto insertion_point = find_node(possible_node->value);
            if (*insertion_point)
            {
                std::destroy_at(possible_node->value);
                return false;
            }

            possible_node->left = nullptr;
            possible_node->right = nullptr;
            *insertion_point = possible_node;
            buffer.release();
            ++element_count;
            return true;
        }

        bool exists(const ValueType& value) const
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

        std::size_t size() const {
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
        void deep_copy(node* dest, node* source)
        {
            if (!source)
                return;

            if (source->left)
            {
                dest->left = new node(source->left->value);
                deep_copy(dest->left, source->left);
            }

            if (source->right)
            {
                dest->right = new node(source->right->value);
                deep_copy(dest->right, source->right);
            }
        }

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
