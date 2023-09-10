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

                    bool add_edge_to_vertex(vertex<T> *, vertex<T> *) override;

                    std::string to_string() const override;
            };

            template<typename T>
            single_linked_list<T>::single_linked_list() : graph<T>() {}

            template<typename T>
            single_linked_list<T>::~single_linked_list() {}

            template<typename T>
            bool single_linked_list<T>::add_edge_to_vertex(vertex<T> *node, vertex<T>* node_to_add) {
                if (node->num_edges() == 0) {
                    node->add_edge(node_to_add);
                    return true;
                } else {
                    return false;
                }
            }

            template<typename T>
            std::string single_linked_list<T>::to_string() const {
                std::stringstream stream;
                for (int i=0; i < graph<T>::node_list.size(); i++) {
                    stream << "(" << graph<T>::node_list[i]->get_id() << ")";
                    i == graph<T>::node_list.size() - 1 ? stream << "" : stream << "->";
                }
                return stream.str();
            }
        }
    }
}