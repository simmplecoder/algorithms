#ifndef ALGORITHMS_BINARY_TREE_ITERATOR_HPP
#define ALGORITHMS_BINARY_TREE_ITERATOR_HPP

#include <stack>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cstddef>
#include <vector>

namespace shino
{

    template <typename Node, typename ValueType = decltype(std::declval<Node>().value)>
    class binary_tree_iterator
    {
        std::stack<Node*, std::vector<Node*>> prev_nodes;
        Node* current_node;
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = ValueType;
        using reference = const value_type&;
        using pointer = const value_type*;
        using distance = std::make_signed_t<std::size_t>;

        binary_tree_iterator() :
                current_node(nullptr)
        {
            prev_nodes.push(nullptr);
        }

        explicit binary_tree_iterator(Node* root)
        {
            prev_nodes.push(nullptr);
            current_node = root;
            if (current_node != nullptr and current_node->left != nullptr)
                ++*this;
        }

        const ValueType& operator*()
        {
            return current_node->value;
        }

        binary_tree_iterator& operator++()
        {
            if (current_node->left != nullptr and prev_nodes.top() != current_node)
            {
                prev_nodes.push(current_node);
                current_node = next(current_node->left);
            }
            else
            {
                if (current_node == prev_nodes.top())
                    prev_nodes.pop();
                if (current_node->right != nullptr)
                    current_node = next(current_node->right);
                else
                    current_node = prev_nodes.top();
            }

            return *this;
        }

        bool operator!=(const binary_tree_iterator& rhs)
        {
            return rhs.current_node != current_node;
        }

    private:
        Node* next(Node* start_pos)
        {
            if (start_pos->left != nullptr)
            {
                prev_nodes.push(start_pos);
                return next(start_pos->left);
            }
            else
                return start_pos;
        }
    };
}

#endif //ALGORITHMS_BINARY_TREE_ITERATOR_HPP
