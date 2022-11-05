#pragma once

#include "array.hh"
#include "edge.hh"

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class vertex {
                private:
                    std::string id;
                    T value;
                    array<edge<vertex<T> *>> edges;                    

                public:
                    vertex() = delete;
                    vertex(std::string id, T value) : id{id}, value{value}, edges{} {}

                    void add_node(vertex<T> *node) {
                        auto e = edge<vertex<T>*>(node);
                        edges.add(e);
                    }

                    std::string get_id() { return id; }
                    T get_value() { return value; }

                    std::string to_string() const {
                        std::stringstream stream;
                        stream << id << ":\n\tValue: '" << std::to_string(value) << "'\n\t";
                        stream << "Edges: " << edges << "\n";
                        return stream.str();
                    }
            };

            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const vertex<T>& v) {
                return stream << v.to_string();
            }
        }
    }
}