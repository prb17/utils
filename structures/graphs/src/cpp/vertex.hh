#pragma once

#include "array.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class vertex {
                private:
                    T value;
                    std::string id;
                
                protected:
                    array<vertex<T> *> edges;               

                public:
                    vertex() = delete;
                    vertex(std::string, T);
                    ~vertex();

                    //modifiers
                    void add_edge(vertex<T>*);

                    //accessors
                    T get();
                    size_t num_edges();
                    std::string get_id();
                    array<vertex<T> *> get_edges();
                    vertex<T>* get_connected_vertex(size_t idx);

                    std::string to_string() const;
            };
            
            template<typename T>
            vertex<T>::vertex(std::string id, T value) : id{id}, value{value}, edges{} {}

            template<typename T>
            vertex<T>::~vertex() {}

            template<typename T>
            void vertex<T>::add_edge(vertex<T>* node) {
                edges.add(node);
            }

            //accessors
            template<typename T>
            std::string vertex<T>::get_id() { return id; }
            
            template<typename T>
            T vertex<T>::get() { return value; }

            template<typename T>
            size_t vertex<T>::num_edges() { return edges.size(); }

            template<typename T>
            vertex<T>* vertex<T>::get_connected_vertex(size_t idx) {
                return edges.size() > 0 ? edges[idx] : nullptr;
            }

            template<typename T>
            array<vertex<T> *> vertex<T>::get_edges() {
                return edges;
            }

            template<typename T>
            std::string vertex<T>::to_string() const {
                std::stringstream stream;
                stream << "Id: " << id;
                stream << "\nValue: " << value;
                for (int i=0; i < edges.size(); i++) {
                    stream << "\nEdges: " << edges[i]->get_id() << " ";
                }
                
                stream << "\n";
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

            //Weighted vertex 
            template<typename T>
            class weighted_vertex : public vertex<T> {
                private:

                public:
                    weighted_vertex() = delete;
                    weighted_vertex(std::string, T);
                    ~weighted_vertex();

                    void add_edge(weighted_vertex<T>*);
            };

            template<typename T>
            weighted_vertex<T>::weighted_vertex(std::string id, T value)  : vertex<T>(id, value) {}

            template<typename T>
            weighted_vertex<T>::~weighted_vertex() {}
            
            template<typename T>
            void weighted_vertex<T>::add_edge(weighted_vertex<T>* e) {
                this->edges.add(e);
            }
            
            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const weighted_vertex<T>& v) {
                return stream << v.to_string();
            }
        }
    }
}
