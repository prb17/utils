#pragma once

#include "vertex.hh"
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

                protected:

                public:
                    graph();
                    ~graph();
                    void cleanup();

                    size_t get_count() const;
                    vertex<T>* get(std::string id) const;
                    vertex<T>* at(size_t idx) const;

                    bool add(vertex<T> *v);
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
                // Reject duplicate ids - storage must not hold two vertices with the same id
                if (get(vertex->get_id()) != nullptr) { return false; }

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
            vertex<T>* graph<T>::at(size_t idx) const {
                return ( idx < vertices.size() ) ? vertices[idx] : nullptr;
            }
        }
    }
}
