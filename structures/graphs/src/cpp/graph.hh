#pragma once

#include "vertex.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class graph {
                private:
                    vertex<T> *root;

                public:
                    graph() = delete;
                    graph(vertex<T> *);
                    
                    std::string to_string() const;
            };
            
            template<typename T>
            graph<T>::graph(vertex<T> *r) {
                root = r;
            }
            
            template<typename T>
            std::string graph<T>::to_string() const {
                std::stringstream stream;
                stream << *root;
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