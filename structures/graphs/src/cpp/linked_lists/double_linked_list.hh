#pragma once

#include "single_linked_list.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class double_linked_list : public single_linked_list<T> {
                private:

                protected:

                public:
                    double_linked_list();
                    ~double_linked_list();
                    
                    bool add(vertex<T> *) override;
                    vertex<T>* prev(vertex<T> *) const;                    

                    std::string to_string() const override;
            };

            template<typename T>
            double_linked_list<T>::double_linked_list() : single_linked_list<T>() {}

            template<typename T>
            double_linked_list<T>::~double_linked_list() {}

            template<typename T>
            bool double_linked_list<T>::add(vertex<T> *node) {
                if (node->num_edges() == 2 && graph<T>::add_vertex(node)) {
                    if (this->head == nullptr) {
                        this->head = node;
                    }

                    if (this->tail == nullptr) {
                        this->tail = node;
                    } else {
                        this->tail->remove_edge(0);
                        this->tail->insert_edge(0, node);
                        node->remove_edge(1);
                        node->insert_edge(1, this->tail);
                        this->tail = node;
                    }
                } else {
                    return false;
                }

                return true;
            }

            template<typename T>
            vertex<T>* double_linked_list<T>::prev(vertex<T> *node) const {
                return node->get_connected_vertex(1);
            }

            template<typename T>
            std::string double_linked_list<T>::to_string() const {
                std::stringstream stream;
                vertex<T>* curr_node = this->head;
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