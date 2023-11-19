#pragma once

#include "vertex.hh"
#include "queue.hh"
#include "array.hh"

#include "structures_builder.hh"
#include "concrete_builder_role.hh"

#include<utility>
#include<map>

namespace prb17 {
    namespace utils {
        namespace structures {
            template<typename T>
            class graph {
                private:
                    size_t count;
                    array<vertex<T>*> vertices;

                    void to_string(std::stringstream&, vertex<T>*, queue<vertex<T>*>&) const;     
                protected:

                public:
                    graph();
                    ~graph();
                    void cleanup();
                    
                    size_t get_count() const;
                    vertex<T>* get(std::string id) const;

                    bool add(vertex<T> *v);
                    
                    virtual std::string to_string() const;                    
                    std::string to_adjacency_list() const;
            };

            template<typename T>
            graph<T>::graph() : count{0}, vertices{} {}

            template<typename T>
            graph<T>::~graph() {}

            template<typename T>
            void graph<T>::cleanup() {
                for (int i=0; i<vertices.size(); i++) {
                    delete vertices[i];
                }
            }

            template<typename T>
            size_t graph<T>::get_count() const {
                return count;
            }

            template<typename T>
            bool graph<T>::add(vertex<T> *vertex) {
                if (vertex == nullptr) { return false; }
                //todo: find if id is already present in vertices
                
                vertices.add(vertex);
                count++;
                return true;
            }

            template<typename T>
            vertex<T>* graph<T>::get(std::string id) const {
                size_t i=0;
                while(i < vertices.size() && id != vertices[i]->get_id()) {
                    i++;
                }
                return ( i < get_count() ) ? vertices[i] : nullptr;
            }

            template<typename T>
            std::string graph<T>::to_string() const {
                std::stringstream stream;
                queue<vertex<T>*> visited_nodes{};
                to_string(stream, vertices[0], visited_nodes);
                return stream.str();
            }

            template<typename T>
            void graph<T>::to_string(std::stringstream& prefix, vertex<T>* node, queue<vertex<T>*>& visited) const {
                if (visited.find(node) == -1) {
                    visited.enqueue(node);

                    // Print the current node
                    prefix << "Node: " << node << " | Neighbors: ";

                    // Print the neighbors of the current node
                    for (int j=0; j<node->num_edges(); j++) {
                        vertex<T>* neighbor = node->get_connected_vertex(j);
                        prefix <<  neighbor << " ";                         
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
                for(int i=0; i < vertices.size(); i++) {
                    stream << "Node: " << vertices[i]->get_id() << " | ";
                    for(int j=0; j < vertices[i]->get_edges().size(); j++) {
                        if (vertices[i]->get_edges()[j] != nullptr) {
                            stream << vertices[i]->get_edges()[j]->get_id() << " "; 
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