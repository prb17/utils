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
                    array<edge<vertex<T> >*> edges;                    

                public:
                    vertex() = delete;
                    vertex(std::string id, T value) : id{id}, value{value}, edges{} {}

                    //modifiers
                    //todo: determine if I need to keep weighted and normal edges separate
                    //  Does it not make sense to mix the two?
                    //  Are there any benefits (other than simplicity) to having a graph of nodes with mixed edge types?
                    void add_edge(vertex<T> *node) {
                        auto e = new edge<vertex<T> >(node);
                        edges.add(e);
                    }

                    void add_edge(vertex<T> *node, int weight) {
                        auto e = new weighted_edge<vertex<T> >(weight, node);
                        edges.add(e);
                    }

                    //accessors
                    std::string get_id() { return id; }
                    T get_value() { return value; }

                    std::string to_string() const {
                        std::stringstream stream;
                        stream << id;
                        stream << "\n\tValue: " << value;
                        stream << "\n\tEdges: " << edges << "\n";
                        return stream.str();
                    }
            };

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