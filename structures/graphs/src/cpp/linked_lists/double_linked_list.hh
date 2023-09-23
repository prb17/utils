#pragma once

#include "single_linked_list.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class double_linked_list : public single_linked_list<T> {
                private:
                public:
                    double_linked_list();
                    ~double_linked_list();

                    bool prev(vertex<T> *, vertex<T> *);
                    vertex<T>* prev(vertex<T> *) const;
                    

                    std::string to_string() const override;
            };

            template<typename T>
            double_linked_list<T>::double_linked_list() : single_linked_list<T>() {}

            template<typename T>
            double_linked_list<T>::~double_linked_list() {}            

            template<typename T>
            bool double_linked_list<T>::prev(vertex<T> *node, vertex<T> *node_to_add) {
                if (node != nullptr && node->num_edges() == 1) {
                    graph<T>::add_edge_to_vertex(node, node_to_add);
                    return true;
                } else {
                    return false;
                }
            }

            template<typename T>
            vertex<T>* double_linked_list<T>::prev(vertex<T> *node) const {
                return node->get_edges()[1];
            }

            template<typename T>
            std::string double_linked_list<T>::to_string() const {
                std::stringstream stream;
                vertex<T>* curr_node = this->get_root();
                while (curr_node != nullptr) {
                    stream << "(" << curr_node->get_id() << ")";
                    curr_node = this->next(curr_node);
                    curr_node != nullptr ? stream << "<->" : stream << "";
                }
                return stream.str();
            }
        }
    }
}