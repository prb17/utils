#pragma once

#include "array.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class vertex : public container<T> {
                private:
                    std::string id;
                    array<vertex<T> *> edges;
                    array<int> weights;
                
                protected:            

                public:
                    vertex() = delete;
                    vertex(std::string, T);
                    vertex(std::string, T, size_t);
                    ~vertex();

                    //modifiers
                    void add_edge(vertex<T>*, int weight=1);
                    bool remove_edge(size_t);
                    bool insert_edge(size_t, vertex<T>*);

                    //accessors
                    T get();
                    size_t num_edges() const;
                    std::string get_id() const;
                    array<vertex<T> *> get_edges() const;
                    vertex<T>* get_connected_vertex(size_t idx);

                    virtual std::string to_string() const;
            };
            
            template<typename T>
            vertex<T>::vertex(std::string id, T value) : vertex<T>(id, value, 0) {}
            
            template<typename T>
            vertex<T>::vertex(std::string id, T value, size_t num_starting_edges) : id{id}, container<T>{value}, edges{num_starting_edges}, weights{num_starting_edges} {}

            template<typename T>
            vertex<T>::~vertex() {}
                        
            template<typename T>
            void vertex<T>::add_edge(vertex<T>* e, int weight) {
                edges.add(e);
                weights.add(weight);
            }

            template<typename T>
            bool vertex<T>::remove_edge(size_t index) {
                edges.remove(index);
                return true;
            }

            template<typename T>
            bool vertex<T>::insert_edge(size_t index, vertex<T>* node) {
                edges.insert(index, node);
                return true;
            }

            //accessors
            template<typename T>
            std::string vertex<T>::get_id() const { return id; }
            
            template<typename T>
            T vertex<T>::get() { return this->value(); }

            template<typename T>
            size_t vertex<T>::num_edges() const { return edges.size(); }

            template<typename T>
            vertex<T>* vertex<T>::get_connected_vertex(size_t idx) {
                return edges.size() > 0 ? edges[idx] : nullptr;
            }

            template<typename T>
            array<vertex<T> *> vertex<T>::get_edges() const{
                return edges;
            }

            template<typename T>
            std::string vertex<T>::to_string() const {
                std::stringstream stream;
                stream << "(Id: " << id;
                stream << ", Value: " << this->value();
                for (int i=0; i < edges.size(); i++) {
                    stream << ", Edges: {id: " << edges[i]->get_id() << "} ";
                }
                
                stream << ")";
                return stream.str();
            }

            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const vertex<T>& v) {
                return stream << v.to_string();
            }

            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const vertex<T>* v) {
                return stream << v->to_string();
            }
        }
    }
}
