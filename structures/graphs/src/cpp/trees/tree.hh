#pragma once

#include <string>

#include "graph.hh"
#include "vertex.hh"
#include "array.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            /**
             * @brief A tree is a specialization of a graph: a rooted, acyclic
             *      structure where every node has at most `max_children` children.
             *
             * The tree owns storage and access only (per the structures vs.
             * algorithms layering): it builds the node/edge storage on top of a
             * graph and enforces the tree invariants (single root, unique ids,
             * branching factor). Traversal and rendering live in
             * algorithms/graphs/ and operate on `as_graph()` / the accessors here.
             *
             * A `max_children` of 0 means unbounded. A binary tree is simply a
             * tree with `max_children` set to 2 (see binary_tree.hh).
             */
            template<typename T>
            class tree {
                private:
                    size_t max_child;
                    graph<T> g;
                    std::string root_id;
                    bool rooted;

                protected:

                public:
                    tree();
                    tree(size_t max_children);
                    ~tree();
                    void cleanup();

                    //modifiers
                    bool add_root(std::string id, T value);
                    bool add_child(std::string parent_id, std::string id, T value);

                    //accessors
                    size_t max_children() const;
                    size_t get_count() const;
                    vertex<T>* get(std::string id) const;
                    vertex<T>* get_root() const;
                    array<vertex<T>*> get_children(std::string parent_id) const;
                    bool is_full(std::string parent_id) const;
                    const graph<T>& as_graph() const;
            };

            template<typename T>
            tree<T>::tree() : tree<T>(0) {} // 0 == unbounded branching factor

            template<typename T>
            tree<T>::tree(size_t max_children) : max_child{max_children}, g{}, root_id{}, rooted{false} {}

            template<typename T>
            tree<T>::~tree() {}

            template<typename T>
            void tree<T>::cleanup() {
                g.cleanup();
            }

            //modifiers
            /**
             * @brief sets the single root of the tree. Fails if a root already exists.
             */
            template<typename T>
            bool tree<T>::add_root(std::string id, T value) {
                if (rooted) { return false; }
                vertex<T>* v = new vertex<T>{id, value};
                if (!g.add(v)) { delete v; return false; }
                root_id = id;
                rooted = true;
                return true;
            }

            /**
             * @brief adds a new child under parent_id. Fails when the parent does
             *      not exist, the id is already used, or the parent is already full.
             */
            template<typename T>
            bool tree<T>::add_child(std::string parent_id, std::string id, T value) {
                vertex<T>* parent = g.get(parent_id);
                if (parent == nullptr) { return false; }
                if (g.get(id) != nullptr) { return false; }
                if (max_child != 0 && parent->num_edges() >= max_child) { return false; }

                vertex<T>* child = new vertex<T>{id, value};
                if (!g.add(child)) { delete child; return false; }
                parent->add_edge(child);
                return true;
            }

            //accessors
            template<typename T>
            size_t tree<T>::max_children() const { return max_child; }

            template<typename T>
            size_t tree<T>::get_count() const { return g.get_count(); }

            template<typename T>
            vertex<T>* tree<T>::get(std::string id) const { return g.get(id); }

            template<typename T>
            vertex<T>* tree<T>::get_root() const { return rooted ? g.get(root_id) : nullptr; }

            template<typename T>
            bool tree<T>::is_full(std::string parent_id) const {
                vertex<T>* p = g.get(parent_id);
                if (p == nullptr) { return false; }
                return max_child != 0 && p->num_edges() >= max_child;
            }

            template<typename T>
            array<vertex<T>*> tree<T>::get_children(std::string parent_id) const {
                array<vertex<T>*> children{};
                vertex<T>* p = g.get(parent_id);
                if (p == nullptr) { return children; }
                for (size_t i=0; i<p->num_edges(); i++) {
                    children.add(p->get_connected_vertex(i));
                }
                return children;
            }

            template<typename T>
            const graph<T>& tree<T>::as_graph() const { return g; }
        }
    }
}
