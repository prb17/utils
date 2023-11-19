#pragma once

#include "graph.hh"
#include "vertex.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class single_linked_list : public graph<T> {
                private:
                    size_t get_count() const = delete;

                protected:
                    vertex<T> *head;
                    vertex<T> *tail;

                public:
                    single_linked_list();
                    ~single_linked_list();

                    virtual bool add(vertex<T> *);

                    vertex<T>* next(vertex<T> *) const;
                    size_t num_nodes() const;

                    std::string to_string() const override;
            };

            template<typename T>
            single_linked_list<T>::single_linked_list() : head{nullptr}, tail{head}, graph<T>() {}

            template<typename T>
            single_linked_list<T>::~single_linked_list() {}

            template<typename T>
            size_t single_linked_list<T>::num_nodes() const {
                return graph<T>::get_num_vertices();
            }

            template<typename T>
            bool single_linked_list<T>::add(vertex<T> *node) {
                if (node->num_edges() == 1 && graph<T>::add(node)) {
                    if (head == nullptr) {
                        head = node;
                    }

                    if (tail == nullptr) {
                        tail = node;
                    } else {
                        tail->remove_edge(0);
                        tail->insert_edge(0, node);
                        tail = node;
                    }
                } else {
                    return false;
                }

                return true;
            }

            template<typename T>
            vertex<T>* single_linked_list<T>::next(vertex<T> *node) const {
                return node->get_connected_vertex(0);
            }

            template<typename T>
            std::string single_linked_list<T>::to_string() const {
                std::stringstream stream;
                vertex<T>* curr_node = head;
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