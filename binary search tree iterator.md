## Algorithm

Start with a stack, `prev_nodes`, which contains `nullptr`. Initialize binary_tree_iterator to root of a tree.

## Finding next node

If the left node of `current` is not `nullptr` and `current` != `prev_nodes.top()`, find leftmost node starting from the left of `current`. Assign `current` to the node found.

If the left node is `nullptr` or `current` == `prev_nodes.top()`, then it means that left side either doesn't exist or is already traversed. If `current` == `prev_nodes.top()`, pop it from the stack, as `current` is already visited. Try to go right (find leftmost child of `current->right`), and assign it to `current`. If right child doesn't exist, then it means that upward node is needed. Just `current=prev_nodes.top()`. Note that if the traversing is going upwards, every time `prev_nodes.pop()` will be executed and `current = prev_nodes.top()`.