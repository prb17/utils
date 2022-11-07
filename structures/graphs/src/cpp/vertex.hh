#pragma once

#include "array.hh"
#include "edge.hh"

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class vertex {
                private:
                    T value;
                    std::string id;
                    array<edge<vertex<T> > *> edges;                    

                public:
                    vertex() = delete;
                    vertex(std::string, T);
                    ~vertex();

                    //modifiers
                    void add_edge(vertex<T>*);
                    void add_edge(vertex<T>*, int);

                    //accessors
                    std::string get_id();
                    T get_value();

                    std::string to_string() const;
            };
            
            template<typename T>
            vertex<T>::vertex(std::string id, T value) : id{id}, value{value}, edges{} {}

            template<typename T>
            vertex<T>::~vertex() {
                for(int i=0; i<edges.get_size(); i++) {
                    delete edges.get_value(i);
                }
            }

            //modifiers
            //todo: determine if I need to keep weighted and normal edges separate
            //  Does it not make sense to mix the two?
            //  Are there any benefits (other than simplicity) to having a graph of nodes with mixed edge types?
            template<typename T>
            void vertex<T>::add_edge(vertex<T>* node) {
                auto *e = new edge<vertex<T> >(node);
                edges.add(e);
            }

            template<typename T>
            void vertex<T>::add_edge(vertex<T>* node, int weight) {
                auto *e = new weighted_edge<vertex<T> >(node, weight);
                edges.add(e);
            }

            //accessors
            template<typename T>
            std::string vertex<T>::get_id() { return id; }
            
            template<typename T>
            T vertex<T>::get_value() { return value; }

            template<typename T>
            std::string vertex<T>::to_string() const {
                std::stringstream stream;
                stream << id;
                stream << "\n\tValue: " << value;
                stream << "\n\tEdges: " << edges << "\n";
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