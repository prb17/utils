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
                
                protected:

                public:
                    graph();
                    graph(vertex<T> *);
                    ~graph();
                    
                    virtual std::string to_string() const;
                    void to_string(std::stringstream&, vertex<T>*, queue<vertex<T>*>&) const;
                    std::string to_adjacency_list() const;

                    vertex<T>* get_root() const;
                    void set_root(vertex<T>*);
                    size_t num_nodes() const;

                    virtual bool add_vertex(vertex<T> *);
                    virtual bool add_edge_to_vertex(vertex<T> *, vertex<T> *);
                    vertex<T>* get_vertex(std::string) const;
                    vertex<T>* get_vertex(size_t idx) const;

                    void cleanup();
            };

            template<typename T>
            graph<T>::graph() : graph(nullptr) {}
            
            template<typename T>
            graph<T>::graph(vertex<T> *r) : root{r}, node_list{} {}

            template<typename T>
            graph<T>::~graph() {}

            template<typename T>
            void graph<T>::cleanup() {
                for (int i=0; i<node_list.size(); i++) {
                    delete node_list[i];
                }
            }

            template<typename T>
            size_t graph<T>::num_nodes() const {
                return node_list.size();
            }

            template<typename T>
            bool graph<T>::add_vertex(vertex<T> *v) {
                //todo: return false if id of  v already exists in node_list
                node_list.add(v);
                return true;
            }

            template<typename T>
            bool graph<T>::add_edge_to_vertex(vertex<T> *node, vertex<T> *node_to_add) {
                //todo: check if node_to_add already exists in node edges
                node->add_edge(node_to_add);
                return true;
            }

            template<typename T>
            vertex<T>* graph<T>::get_vertex(std::string id) const {
                size_t i=0;
                while(i < node_list.size() && id != node_list[i]->get_id()) {
                    i++;
                }
                return get_vertex(i);
            }

            template<typename T>
            vertex<T>* graph<T>::get_vertex(size_t idx) const {
                return ( idx < num_nodes() ) ? node_list[idx] : nullptr;
            }
            
            template<typename T>
            std::string graph<T>::to_string() const {
                std::stringstream stream;
                queue<vertex<T>*> visited_nodes{};
                to_string(stream, root, visited_nodes);
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
                for(int i=0; i < node_list.size(); i++) {
                    stream << "Node: " << node_list[i]->get_id() << " | ";
                    for(int j=0; j < node_list[i]->get_edges().size(); j++) {
                            node_list[i]->get_edges()[j] != nullptr ? 
                                stream << node_list[i]->get_edges()[j]->get_id() << " " :
                                stream << "'' "; 
                    }
                    stream << std::endl;
                }                
                return stream.str();
            }
            
            template<typename T>
            vertex<T>* graph<T>::get_root() const {
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

            // Weigted graph 
            template<typename T>
            class weighted_graph : public graph<T> {
                private:

                public:
                    weighted_graph();
                    weighted_graph(weighted_vertex<T> *);
                    ~weighted_graph();

                    void add_vertex(weighted_vertex<T> *);
                    weighted_vertex<T>* get_vertex(std::string id);

            };

            template<typename T>
            weighted_graph<T>::weighted_graph() : graph<T>() {}

            template<typename T>
            weighted_graph<T>::weighted_graph(weighted_vertex<T> *v) : graph<T>(v) {}

            template<typename T>
            weighted_graph<T>::~weighted_graph() {}

            template<typename T>
            void weighted_graph<T>::add_vertex(weighted_vertex<T> *v) {
                graph<T>::add_vertex(v);
            }

            template<typename T>
            weighted_vertex<T>* weighted_graph<T>::get_vertex(std::string id) {
                return (weighted_vertex<T>*)(graph<T>::get_vertex(id));
            }
        }
    }
}