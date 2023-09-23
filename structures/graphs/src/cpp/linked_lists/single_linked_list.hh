#pragma once

#include "graph.hh"
#include "vertex.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class single_linked_list : public graph<T> {
                private:
                public:
                    single_linked_list();
                    ~single_linked_list();

                    bool next(vertex<T> *, vertex<T> *);
                    vertex<T>* next(vertex<T> *) const;

                    std::string to_string() const override;
            };

            template<typename T>
            single_linked_list<T>::single_linked_list() : graph<T>() {}

            template<typename T>
            single_linked_list<T>::~single_linked_list() {}

            template<typename T>
            bool single_linked_list<T>::next(vertex<T> *node, vertex<T> *node_to_add) {
                if (node != nullptr && node->num_edges() == 0) {
                    graph<T>::add_edge_to_vertex(node, node_to_add);
                    return true;
                } else {
                    return false;
                }
            }

            template<typename T>
            vertex<T>* single_linked_list<T>::next(vertex<T> *node) const {
                return node->get_edges()[0];
            }

            template<typename T>
            std::string single_linked_list<T>::to_string() const {
                std::stringstream stream;
                vertex<T>* curr_node = this->get_root();
                while (curr_node != nullptr) {
                    stream << "(" << curr_node->get_id() << ")";
                    curr_node = this->next(curr_node);
                    curr_node != nullptr ? stream << "->" : stream << "";
                }
                return stream.str();
            }
        }
    }
}