#pragma once

#include "vertex.hh"
#include "queue.hh"
#include "array.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class graph {
                private:
                    vertex<T> *root;
                    array<vertex<T>*> node_list;

                public:
                    graph();
                    graph(vertex<T> *);
                    ~graph();
                    
                    std::string to_string() const;
                    void to_string(std::stringstream&, vertex<T>*, prb17::utils::structures::queue<vertex<T>*>&) const;
                    std::string to_adjacency_list() const;

                    vertex<T>* get_root();
                    void set_root(vertex<T>*);

                    void add_vertex(vertex<T> *);
                    vertex<T>* get_vertex(std::string);

                    void cleanup();
            };

            template<typename T>
            graph<T>::graph() : graph(nullptr) {}
            
            template<typename T>
            graph<T>::graph(vertex<T> *r) : root{r}, node_list{} {}

            template<typename T>
            graph<T>::~graph() {
                // for (int i=0; i<node_list.size(); i++) {
                //     delete node_list[i];
                // }
            }

            template<typename T>
            void graph<T>::cleanup() {
                for (int i=0; i<node_list.size(); i++) {
                    delete node_list[i];
                }
            }

            template<typename T>
            void graph<T>::add_vertex(vertex<T> *v) {
                //todo: throw exception if id of v already exists in node_list
                node_list.add(v);
            }

            template<typename T>
            vertex<T>* graph<T>::get_vertex(std::string id) {
                //todo: throw exception if id of v already exists in node_list
                int i=0;
                while(i < node_list.size() && id != node_list[i]->get_id()) {
                    i++;
                }
                return node_list[i];
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
            std::string graph<T>::to_adjacency_list() const {
                std::stringstream stream;
                for(int i=0; i<node_list.size(); i++) {
                    stream << "Node: " << node_list[i]->get_id() << " | Neighbors: " << node_list[i]->get_edges() << std::endl;
                }                
                return stream.str();
            }
            
            template<typename T>
            vertex<T>* graph<T>::get_root() {
                return root;
            }
            template<typename T>
            void graph<T>::set_root(vertex<T> *v) {
                root = v;
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