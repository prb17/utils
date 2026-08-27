#pragma once

#include <functional>
#include <algorithm>
#include <cstddef>

#include "array.hh"
#include "exception.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            /**
             * @brief A self-balancing binary search tree (AVL tree).
             *
             * This is the one container in the library whose invariant genuinely
             * cannot be an external algorithm: the balance property is coupled to
             * every mutation. Each node stores a subtree height, and insert/remove
             * restore the AVL invariant (|balance factor| <= 1 at every node) with
             * rotations as they unwind, keeping all operations O(log n).
             *
             * Values are unique (set semantics); Compare defines the order and the
             * default std::less gives ascending in-order traversal.
             */
            template<typename T, typename Compare = std::less<T>>
            class avl_tree {
                private:
                    struct node {
                        T value;
                        node* left;
                        node* right;
                        int height;
                        node(T v) : value{v}, left{nullptr}, right{nullptr}, height{0} {}
                    };

                    node* root;
                    size_t sz;
                    Compare comp;

                    static int node_height(node* n) { return n ? n->height : -1; }
                    static int balance(node* n) { return n ? node_height(n->left) - node_height(n->right) : 0; }
                    static void update_height(node* n) {
                        n->height = 1 + std::max(node_height(n->left), node_height(n->right));
                    }

                    static node* rotate_right(node* y) {
                        node* x = y->left;
                        node* t2 = x->right;
                        x->right = y;
                        y->left = t2;
                        update_height(y);
                        update_height(x);
                        return x;
                    }

                    static node* rotate_left(node* x) {
                        node* y = x->right;
                        node* t2 = y->left;
                        y->left = x;
                        x->right = t2;
                        update_height(x);
                        update_height(y);
                        return y;
                    }

                    // rebalance a node after its height may have changed
                    static node* rebalance(node* n) {
                        update_height(n);
                        int bf = balance(n);
                        if (bf > 1) {                       // left-heavy
                            if (balance(n->left) < 0) { n->left = rotate_left(n->left); } // LR
                            return rotate_right(n);                                       // LL
                        }
                        if (bf < -1) {                      // right-heavy
                            if (balance(n->right) > 0) { n->right = rotate_right(n->right); } // RL
                            return rotate_left(n);                                            // RR
                        }
                        return n;
                    }

                    node* insert_node(node* n, const T& value) {
                        if (n == nullptr) { return new node{value}; }
                        if (comp(value, n->value)) {
                            n->left = insert_node(n->left, value);
                        } else {
                            n->right = insert_node(n->right, value);
                        }
                        return rebalance(n);
                    }

                    static node* min_node(node* n) {
                        while (n->left != nullptr) { n = n->left; }
                        return n;
                    }

                    node* remove_node(node* n, const T& value) {
                        if (n == nullptr) { return nullptr; }
                        if (comp(value, n->value)) {
                            n->left = remove_node(n->left, value);
                        } else if (comp(n->value, value)) {
                            n->right = remove_node(n->right, value);
                        } else {
                            if (n->left == nullptr || n->right == nullptr) {
                                node* child = n->left ? n->left : n->right;
                                delete n;
                                return child;
                            }
                            node* succ = min_node(n->right);
                            n->value = succ->value;
                            n->right = remove_node(n->right, succ->value);
                        }
                        return rebalance(n);
                    }

                    bool contains_node(node* n, const T& value) const {
                        while (n != nullptr) {
                            if (comp(value, n->value)) { n = n->left; }
                            else if (comp(n->value, value)) { n = n->right; }
                            else { return true; }
                        }
                        return false;
                    }

                    void in_order_node(node* n, array<T>& out) const {
                        if (n == nullptr) { return; }
                        in_order_node(n->left, out);
                        out.add(n->value);
                        in_order_node(n->right, out);
                    }

                    void destroy(node* n) {
                        if (n == nullptr) { return; }
                        destroy(n->left);
                        destroy(n->right);
                        delete n;
                    }

                public:
                    avl_tree() : root{nullptr}, sz{0}, comp{} {}
                    avl_tree(Compare c) : root{nullptr}, sz{0}, comp{c} {}
                    ~avl_tree() { destroy(root); }

                    // non-copyable: owns raw nodes
                    avl_tree(const avl_tree&) = delete;
                    avl_tree& operator=(const avl_tree&) = delete;

                    //modifiers
                    /**
                     * @brief inserts value if not already present. Returns false when
                     *      the value is a duplicate (set semantics).
                     */
                    bool insert(T value) {
                        if (contains(value)) { return false; }
                        root = insert_node(root, value);
                        sz++;
                        return true;
                    }

                    /**
                     * @brief removes value if present. Returns false when absent.
                     */
                    bool remove(T value) {
                        if (!contains(value)) { return false; }
                        root = remove_node(root, value);
                        sz--;
                        return true;
                    }

                    void clear() {
                        destroy(root);
                        root = nullptr;
                        sz = 0;
                    }

                    //accessors
                    bool contains(const T& value) const { return contains_node(root, value); }

                    /**
                     * @brief the values in sorted order.
                     */
                    array<T> in_order() const {
                        array<T> out{};
                        in_order_node(root, out);
                        return out;
                    }

                    /**
                     * @brief height in edges: an empty tree is -1, a single node is 0.
                     */
                    int height() const { return node_height(root); }

                    size_t size() const { return sz; }
                    bool empty() const { return sz == 0; }

                    T min() const {
                        if (root == nullptr) { throw utils::exception("min of empty tree"); }
                        return min_node(root)->value;
                    }

                    T max() const {
                        if (root == nullptr) { throw utils::exception("max of empty tree"); }
                        node* n = root;
                        while (n->right != nullptr) { n = n->right; }
                        return n->value;
                    }
            };
        }
    }
}
