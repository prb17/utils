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
                    size_t num_vertices;
                    array<vertex<T>*> vertex_list;

                    void to_string(std::stringstream&, vertex<T>*, queue<vertex<T>*>&) const;     
                protected:

                public:
                    graph();
                    ~graph();
                    void cleanup();
                    
                    size_t get_num_vertices() const;
                    vertex<T>* get_vertex(std::string id) const;

                    bool add_vertex(vertex<T> *v);
                    
                    virtual std::string to_string() const;                    
                    std::string to_adjacency_list() const;
            };

            template<typename T>
            graph<T>::graph() : num_vertices{0}, vertex_list{} {}

            template<typename T>
            graph<T>::~graph() {}

            template<typename T>
            void graph<T>::cleanup() {
                for (int i=0; i<vertex_list.size(); i++) {
                    delete vertex_list[i];
                }
            }

            template<typename T>
            size_t graph<T>::get_num_vertices() const {
                return num_vertices;
            }

            template<typename T>
            bool graph<T>::add_vertex(vertex<T> *vertex) {
                if (vertex == nullptr) { return false; }
                //todo: find if id is already present in vertex_list
                
                vertex_list.add(vertex);
                num_vertices++;
                return true;
            }

            template<typename T>
            vertex<T>* graph<T>::get_vertex(std::string id) const {
                size_t i=0;
                while(i < vertex_list.size() && id != vertex_list[i]->get_id()) {
                    i++;
                }
                return ( i < get_num_vertices() ) ? vertex_list[i] : nullptr;
            }

            template<typename T>
            std::string graph<T>::to_string() const {
                std::stringstream stream;
                queue<vertex<T>*> visited_nodes{};
                to_string(stream, vertex_list[0], visited_nodes);
                return stream.str();
            }

            template<typename T>
            void graph<T>::to_string(std::stringstream& prefix, vertex<T>* node, queue<vertex<T>*>& visited) const {
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
                for(int i=0; i < vertex_list.size(); i++) {
                    stream << "Node: " << vertex_list[i]->get_id() << " | ";
                    for(int j=0; j < vertex_list[i]->get_edges().size(); j++) {
                        if (vertex_list[i]->get_edges()[j] != nullptr) {
                            stream << vertex_list[i]->get_edges()[j]->get_id() << " "; 
                        }                                
                    }
                    stream << std::endl;
                }                
                return stream.str();
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