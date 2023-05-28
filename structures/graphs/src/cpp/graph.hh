#pragma once

#include "vertex.hh"
#include "queue.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class graph {
                private:
                    vertex<T> *root;

                public:
                    graph() = delete;
                    graph(vertex<T> *);
                    
                    std::string to_string() const;
                    void to_string(std::stringstream&, vertex<T>*, prb17::utils::structures::queue<vertex<T>*>&) const;
                    vertex<T>* get_root();
            };
            
            template<typename T>
            graph<T>::graph(vertex<T> *r) {
                root = r;
            }
            
            template<typename T>
            std::string graph<T>::to_string() const {
                std::stringstream stream;
                prb17::utils::structures::queue<vertex<T>*> visited_nodes{};
                to_string(stream, root, visited_nodes);
                return stream.str();
            }

            template<typename T>
            void graph<T>::to_string(std::stringstream& prefix, vertex<T>* node, prb17::utils::structures::queue<vertex<T>*>& visited) const {
                if (visited.find(node) == -1) {
                    visited.enqueue(node);

                    // Print the current node
                    prefix << "Node: " << node->get() << " | Neighbors: ";

                    // Print the neighbors of the current node
                    for (int j=0; j<node->num_edges(); j++) {
                        vertex<T>* neighbor = node->get_connected_vertex(j);
                        prefix <<  neighbor->get() << " ";                         
                    }
                    prefix << std::endl;

                    // Recursively traverse the unvisited neighbors
                    for (int j=0; j<node->num_edges(); j++) {
                        vertex<T>* neighbor = node->get_connected_vertex(j);
                        if (visited.find(neighbor) == -1) {
                            to_string(prefix, neighbor, visited);
                        }
                    }
                }
            }
            
            template<typename T>
            vertex<T>* graph<T>::get_root() {
                return root;
            }

            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const graph<T>& graph) {
                return stream << graph.to_string();
            }

            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const graph<T>* graph) {
                return stream << graph->to_string();
            }
        }
    }
}