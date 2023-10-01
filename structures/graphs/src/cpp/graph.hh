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

                    bool add_vertex(std::string, T);
                    size_t get_num_vertices() const;
                    vertex<T>* get_vertex(std::string id) const;
                    vertex<T>* get_vertex(size_t index) const;

                    bool add_edge_to_vertex(vertex<T> *, vertex<T> *);
                    
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
            bool graph<T>::add_vertex(std::string id, T value) {
                if (id.empty()) { return false; }
                //todo: find if id is already present in vertex_list
                
                vertex<T> *v = new vertex<T>(id, value);
                vertex_list.add(v);
                num_vertices++;
                return true;
            }

            template<typename T>
            bool graph<T>::add_edge_to_vertex(vertex<T> *v, vertex<T> *v2) {
                if (v == nullptr) { return false; }
                
                v->add_edge(v2);
                return true;
            }

            template<typename T>
            vertex<T>* graph<T>::get_vertex(std::string id) const {
                size_t i=0;
                while(i < vertex_list.size() && id != vertex_list[i]->get_id()) {
                    i++;
                }
                return get_vertex(i);
            }

            template<typename T>
            vertex<T>* graph<T>::get_vertex(size_t idx) const {
                return ( idx < get_num_vertices() ) ? vertex_list[idx] : nullptr;
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
                            vertex_list[i]->get_edges()[j] != nullptr ? 
                                stream << vertex_list[i]->get_edges()[j]->get_id() << " " :
                                stream << "'' "; 
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

            // Weigted graph 
            template<typename T>
            class weighted_graph : public graph<T> {
                private:

                public:
                    weighted_graph();
                    weighted_graph(weighted_vertex<T> *);
                    ~weighted_graph();

                    // bool add_vertex(std::string, T) override;
                    weighted_vertex<T>* get_vertex(std::string id);

            };

            template<typename T>
            weighted_graph<T>::weighted_graph() {}

            template<typename T>
            weighted_graph<T>::~weighted_graph() {}

            // template<typename T>
            // bool weighted_graph<T>::add_vertex(std::string id, T value) {
            //     return graph<T>::add_vertex(id, value);
            // }

            template<typename T>
            weighted_vertex<T>* weighted_graph<T>::get_vertex(std::string id) {
                return (weighted_vertex<T>*)(graph<T>::get_vertex(id));
            }
        }
    }
}