#pragma once

#include "vertex.hh"

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class graph {
                private:
                    vertex<T> *root;

                public:
                    graph() = delete;
                    graph(vertex<T> *r) {
                        root = r;
                    }
                    
                    std::string to_string() const {
                        std::stringstream stream;
                        stream << *root;
                        return stream.str();
                    }
            };

            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const graph<T>& graph) {
                return stream << graph.to_string();
            }
        }
    }
}