#pragma once

#include <functional>
#include <string>
#include <atomic>

#include "graph.hh"
#include "vertex.hh"
#include "array.hh"

#include "search.hh"

namespace prb17 {
    namespace utils {
        namespace algorithms {
            namespace graphs {
                /**
                 * @brief Binary search tree behavior implemented as algorithms over a
                 *      plain graph container.
                 *
                 * The container stays a graph<T>: each BST node is a vertex, and a
                 * parent points to its children via edges. A node's *side* is not
                 * stored -- it is recovered from the BST property itself: a child whose
                 * value is less than its parent is the left child, otherwise the right
                 * (equal values go right). So these algorithms give a graph BST
                 * semantics without the graph knowing anything about ordering.
                 *
                 * Usage:
                 *      graph<int> bst;
                 *      namespace ops = prb17::utils::algorithms::graphs::bst;
                 *      ops::insert(bst, 10);
                 *      ops::contains(bst, 10);
                 *      ops::validate(bst);
                 *      ops::remove(bst, 10);
                 *      bst.cleanup();
                 */
                namespace bst {

                    using prb17::utils::structures::graph;
                    using prb17::utils::structures::vertex;
                    using prb17::utils::structures::array;

                    // Vertices need a unique id; the BST keys on value, so the id is
                    // just an opaque, unique handle.
                    inline std::string next_node_id() {
                        static std::atomic<size_t> counter{0};
                        return "bst-" + std::to_string(counter++);
                    }

                    // The root is the unique vertex that is nobody's child. Found by
                    // membership through algorithms::search::find so the search stays
                    // swappable (and to keep the graph free of a stored root).
                    template<typename T>
                    vertex<T>* root(const graph<T>& g) {
                        if (g.get_count() == 0) { return nullptr; }
                        array<vertex<T>*> children{};
                        for (size_t i=0; i<g.get_count(); i++) {
                            vertex<T>* v = g.at(i);
                            for (size_t j=0; j<v->num_edges(); j++) {
                                children.add(v->get_connected_vertex(j));
                            }
                        }
                        for (size_t i=0; i<g.get_count(); i++) {
                            vertex<T>* v = g.at(i);
                            if (search::find(children, v) == -1) { return v; }
                        }
                        return nullptr; // no root => every node has a parent (a cycle)
                    }

                    template<typename T, typename Compare>
                    vertex<T>* left_child(vertex<T>* node, const Compare& comp) {
                        for (size_t j=0; j<node->num_edges(); j++) {
                            vertex<T>* c = node->get_connected_vertex(j);
                            if (comp(c->get(), node->get())) { return c; } // c < node
                        }
                        return nullptr;
                    }

                    template<typename T, typename Compare>
                    vertex<T>* right_child(vertex<T>* node, const Compare& comp) {
                        for (size_t j=0; j<node->num_edges(); j++) {
                            vertex<T>* c = node->get_connected_vertex(j);
                            if (!comp(c->get(), node->get())) { return c; } // c >= node
                        }
                        return nullptr;
                    }

                    template<typename T>
                    void remove_edge_between(vertex<T>* parent, vertex<T>* child) {
                        for (size_t j=0; j<parent->num_edges(); j++) {
                            if (parent->get_connected_vertex(j) == child) {
                                parent->remove_edge(j);
                                return;
                            }
                        }
                    }

                    /**
                     * @brief inserts value, keeping the BST property (equal values go
                     *      right). Grows the graph by one vertex.
                     */
                    template<typename T, typename Compare = std::less<T>>
                    void insert(graph<T>& g, T value, Compare comp = Compare{}) {
                        if (g.get_count() == 0) {
                            g.add(new vertex<T>{next_node_id(), value});
                            return;
                        }
                        vertex<T>* cur = root(g);
                        while (true) {
                            bool go_left = comp(value, cur->get());
                            vertex<T>* next = go_left ? left_child(cur, comp) : right_child(cur, comp);
                            if (next == nullptr) {
                                vertex<T>* node = new vertex<T>{next_node_id(), value};
                                g.add(node);
                                cur->add_edge(node);
                                return;
                            }
                            cur = next;
                        }
                    }

                    /**
                     * @brief returns the vertex holding value (the topmost such node),
                     *      or nullptr if absent. Walks the tree by comparison, O(h).
                     */
                    template<typename T, typename Compare = std::less<T>>
                    vertex<T>* find(const graph<T>& g, T value, Compare comp = Compare{}) {
                        vertex<T>* cur = root(g);
                        while (cur != nullptr) {
                            if (comp(value, cur->get())) {
                                cur = left_child(cur, comp);
                            } else if (comp(cur->get(), value)) {
                                cur = right_child(cur, comp);
                            } else {
                                return cur;
                            }
                        }
                        return nullptr;
                    }

                    template<typename T, typename Compare = std::less<T>>
                    bool contains(const graph<T>& g, T value, Compare comp = Compare{}) {
                        return find(g, value, comp) != nullptr;
                    }

                    template<typename T, typename Compare>
                    void in_order_helper(vertex<T>* node, const Compare& comp, array<T>& out) {
                        if (node == nullptr) { return; }
                        in_order_helper(left_child(node, comp), comp, out);
                        out.add(node->get());
                        in_order_helper(right_child(node, comp), comp, out);
                    }

                    /**
                     * @brief the values in sorted order (in-order traversal). On a valid
                     *      BST this is non-decreasing.
                     */
                    template<typename T, typename Compare = std::less<T>>
                    array<T> in_order(const graph<T>& g, Compare comp = Compare{}) {
                        array<T> out{};
                        in_order_helper(root(g), comp, out);
                        return out;
                    }

                    template<typename T, typename Compare>
                    bool validate_helper(vertex<T>* node, const T* lo, const T* hi,
                                         const Compare& comp, array<vertex<T>*>& visited) {
                        if (node == nullptr) { return true; }
                        if (search::find(visited, node) != -1) { return false; } // cycle
                        visited.add(node);

                        T val = node->get();
                        if (lo != nullptr && comp(val, *lo)) { return false; }   // val < lo
                        if (hi != nullptr && !comp(val, *hi)) { return false; }  // val >= hi

                        // each node has at most one left and one right child
                        vertex<T>* l = nullptr;
                        vertex<T>* r = nullptr;
                        for (size_t j=0; j<node->num_edges(); j++) {
                            vertex<T>* c = node->get_connected_vertex(j);
                            if (comp(c->get(), val)) {
                                if (l != nullptr) { return false; }
                                l = c;
                            } else {
                                if (r != nullptr) { return false; }
                                r = c;
                            }
                        }
                        // left subtree in [lo, val); right subtree in [val, hi)
                        if (!validate_helper(l, lo, &val, comp, visited)) { return false; }
                        if (!validate_helper(r, &val, hi, comp, visited)) { return false; }
                        return true;
                    }

                    /**
                     * @brief true if the graph is a valid BST: a single rooted tree in
                     *      which every node's value is greater than all values to its
                     *      left and less than or equal to all values to its right.
                     */
                    template<typename T, typename Compare = std::less<T>>
                    bool validate(const graph<T>& g, Compare comp = Compare{}) {
                        if (g.get_count() == 0) { return true; }
                        vertex<T>* r = root(g);
                        if (r == nullptr) { return false; } // cycle among all nodes
                        array<vertex<T>*> visited{};
                        const T* unbounded = nullptr;
                        if (!validate_helper(r, unbounded, unbounded, comp, visited)) { return false; }
                        // every vertex must be reachable from the one root
                        return visited.size() == g.get_count();
                    }

                    /**
                     * @brief removes the first node holding value, keeping the tree a
                     *      valid BST, and deletes that node from the graph. Returns
                     *      false if value is absent.
                     */
                    template<typename T, typename Compare = std::less<T>>
                    bool remove(graph<T>& g, T value, Compare comp = Compare{}) {
                        vertex<T>* parent = nullptr;
                        vertex<T>* cur = root(g);
                        while (cur != nullptr) {
                            if (comp(value, cur->get())) {
                                parent = cur;
                                cur = left_child(cur, comp);
                            } else if (comp(cur->get(), value)) {
                                parent = cur;
                                cur = right_child(cur, comp);
                            } else {
                                break; // found
                            }
                        }
                        if (cur == nullptr) { return false; }

                        vertex<T>* l = left_child(cur, comp);
                        vertex<T>* r = right_child(cur, comp);

                        if (l != nullptr && r != nullptr) {
                            // two children: replace value with the in-order successor
                            // (leftmost node of the right subtree), then splice out the
                            // successor (which has no left child)
                            vertex<T>* succ_parent = cur;
                            vertex<T>* succ = r;
                            while (left_child(succ, comp) != nullptr) {
                                succ_parent = succ;
                                succ = left_child(succ, comp);
                            }
                            vertex<T>* succ_right = right_child(succ, comp);
                            cur->value() = succ->get();
                            remove_edge_between(succ_parent, succ);
                            if (succ_right != nullptr) { succ_parent->add_edge(succ_right); }
                            g.remove(succ);
                            return true;
                        }

                        // zero or one child
                        vertex<T>* child = (l != nullptr) ? l : r;
                        if (parent != nullptr) {
                            remove_edge_between(parent, cur);
                            if (child != nullptr) { parent->add_edge(child); }
                        }
                        // if cur is the root, its child (if any) loses its only parent
                        // when cur is deleted and simply becomes the new root
                        g.remove(cur);
                        return true;
                    }
                }
            }
        }
    }
}
