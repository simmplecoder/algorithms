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
    public:
        using iterator = binary_tree_iterator<node>;

        binary_search_tree() :
                root(nullptr)
        {}

        binary_search_tree(const binary_search_tree& other) = delete;

        binary_search_tree& operator=(const binary_search_tree& other) = delete;

        binary_search_tree(binary_search_tree&& other) noexcept:
                root(std::exchange(other.root, nullptr))
        {}

        binary_search_tree& operator=(binary_search_tree&& other) noexcept
        {
            std::swap(root, other.root);
            return *this;
        }

        bool try_insert(const ValueType& value)
        {
            return try_insert_helper(value, root);
        }

        bool exists(const ValueType& value)
        {
            return find_node(value, nullptr, root, direction::is_root).target_child != nullptr;
        }

        bool delete_if_exists(const ValueType& value)
        {
            auto[parent_node, node_with_value, parent_to_child] =
            find_node(value, nullptr, root, direction::is_root);

            if (node_with_value == nullptr)
                return false;

            if (node_with_value->left == nullptr)
            {
                auto old = node_with_value;
                switch (parent_to_child)
                {
                    case direction::left:
                        parent_node->left = node_with_value->left;
                        break;
                    case direction::right:
                        parent_node->right = node_with_value->right;
                        break;
                    case direction::is_root:
                        root = root->right;
                }
                delete old;
                return true;
            }

            if (node_with_value->left->right == nullptr)
            {
                switch (parent_to_child)
                {
                    case direction::left:
                        parent_node->left = node_with_value->right;
                        node_with_value->right->left = node_with_value->left;
                        break;
                    case direction::right:
                        parent_node->right = node_with_value->right;
                        node_with_value->right->left = node_with_value->left;
                        break;
                    case direction::is_root:
                        root->left->right = root->right;
                        root = root->left;
                }
                delete node_with_value;
                return true;
            }

            auto[suitable_parent, suitable_node] =
            find_suitable_node(node_with_value->left->right, node_with_value->left);
            switch (parent_to_child)
            {
                case direction::left:
                    parent_node->left = suitable_node;
                    suitable_node->right = node_with_value->right;
                    suitable_node->left = node_with_value->left;
                    break;
                case direction::right:
                    parent_node->right = suitable_node;
                    suitable_node->right = node_with_value->right;
                    suitable_node->left = node_with_value->left;

                    break;
                case direction::is_root:
                    suitable_node->right = root->right;
                    suitable_node->left = root->left;
                    root = suitable_node;
            }
            suitable_parent->right = nullptr;
            delete node_with_value;

            return true;
        }

        void clear()
        {
            clear_helper(root);
        }

        void inorder_print(std::ostream& os)
        {
            for (const auto& x: *this)
            {
                os << x << ' ';
            }
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
        std::pair<node*, node*> find_suitable_node(node* start_position, node* parent)
        {
            if (start_position->right == nullptr)
                return {parent, start_position};
            return find_suitable_node(start_position->right, start_position);
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
