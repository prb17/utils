#pragma once

#include <functional>

namespace prb17 {
    namespace utils {
        namespace structures {

            /**
             * @brief A (plain, unbalanced) binary search tree node -- the classic
             *      "BST Construction" exercise.
             *
             * This is a direct, self-contained node class: every node carries a value
             * and owns its left and right children. It satisfies the BST property --
             * a node's value is strictly greater than every value in its left subtree
             * and less than or equal to every value in its right subtree (so equal
             * values go right).
             *
             * Unlike avl_tree it does no balancing, so its shape (and therefore the
             * cost of its operations) depends on insertion order -- that is the point
             * of the exercise. It lives here with insert/contains/remove as methods
             * for the same reason avl_tree does: the ordering invariant is coupled to
             * every mutation.
             *
             * Compare defines the order; the default std::less matches the exercise's
             * "< goes left, >= goes right" rule.
             */
            template<typename T, typename Compare = std::less<T>>
            class bst {
                private:
                    Compare comp;

                    static bool equal(const T& a, const T& b, const Compare& comp) {
                        return !comp(a, b) && !comp(b, a);
                    }

                    // remove `value` from the subtree rooted at `node`; returns the new
                    // subtree root and deletes the node that is unlinked (C++ has no GC,
                    // so removal must free the node the Python version left to the GC)
                    static bst<T, Compare>* remove_node(bst<T, Compare>* node, const T& value, const Compare& comp) {
                        if (node == nullptr) {
                            return nullptr;
                        }

                        if (comp(value, node->value)) {          // value < node->value
                            node->left = remove_node(node->left, value, comp);
                        } else if (comp(node->value, value)) {   // node->value < value
                            node->right = remove_node(node->right, value, comp);
                        } else {
                            // this is the node to remove
                            if (node->left == nullptr && node->right == nullptr) {
                                delete node;
                                return nullptr;
                            }
                            if (node->left == nullptr) {
                                bst<T, Compare>* right_child = node->right;
                                node->right = nullptr;
                                delete node;
                                return right_child;
                            }
                            if (node->right == nullptr) {
                                bst<T, Compare>* left_child = node->left;
                                node->left = nullptr;
                                delete node;
                                return left_child;
                            }

                            // two children: replace value with the in-order successor
                            // (leftmost node of the right subtree), then remove that
                            // successor value from the right subtree
                            bst<T, Compare>* successor = node->right;
                            while (successor->left != nullptr) {
                                successor = successor->left;
                            }
                            node->value = successor->value;
                            node->right = remove_node(node->right, successor->value, comp);
                        }

                        return node;
                    }

                public:
                    T value;
                    bst<T, Compare>* left;
                    bst<T, Compare>* right;

                    explicit bst(T value) : comp{}, value{value}, left{nullptr}, right{nullptr} {}
                    bst(T value, Compare c) : comp{c}, value{value}, left{nullptr}, right{nullptr} {}

                    // owns its children; non-copyable to avoid double-free
                    bst(const bst&) = delete;
                    bst& operator=(const bst&) = delete;

                    ~bst() {
                        delete left;
                        delete right;
                    }

                    /**
                     * @brief inserts value, keeping the BST property (equal values go
                     *      right). Returns this node so calls can be chained.
                     */
                    bst<T, Compare>* insert(T value) {
                        if (comp(value, this->value)) {
                            if (left == nullptr) {
                                left = new bst<T, Compare>{value, comp};
                            } else {
                                left->insert(value);
                            }
                        } else {
                            if (right == nullptr) {
                                right = new bst<T, Compare>{value, comp};
                            } else {
                                right->insert(value);
                            }
                        }
                        return this;
                    }

                    /**
                     * @brief true if value is present anywhere in the tree.
                     */
                    bool contains(T value) {
                        bool left_has = false;
                        bool right_has = false;
                        if (equal(this->value, value, comp)) {
                            return true;
                        }
                        if (left != nullptr) {
                            left_has = left->contains(value);
                        }
                        if (right != nullptr) {
                            right_has = right->contains(value);
                        }
                        return left_has || right_has;
                    }

                    /**
                     * @brief removes the first instance of value and returns the new
                     *      root of the tree (which may differ from this node, or be
                     *      nullptr if the last node was removed). Removing a value that
                     *      is absent leaves the tree unchanged.
                     *
                     * Use as: root = root->remove(value);
                     */
                    bst<T, Compare>* remove(T value) {
                        return remove_node(this, value, comp);
                    }
            };
        }
    }
}
