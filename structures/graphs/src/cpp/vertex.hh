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
                    bool insert_edge(size_t, vertex<T>*, int weight=1);

                    //accessors
                    T get();
                    size_t num_edges() const;
                    std::string get_id() const;
                    array<vertex<T> *> get_edges() const;
                    vertex<T>* get_connected_vertex(size_t idx);
                    int get_weight(size_t idx) const;
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
                if (index >= edges.size()) { return false; }
                // keep edges and weights index-aligned
                edges.remove(index);
                weights.remove(index);
                return true;
            }

            template<typename T>
            bool vertex<T>::insert_edge(size_t index, vertex<T>* node, int weight) {
                // keep edges and weights index-aligned
                bool inserted = edges.insert(index, node);
                if (inserted) {
                    weights.insert(index, weight);
                }
                return inserted;
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
                return idx < edges.size() ? edges[idx] : nullptr;
            }

            template<typename T>
            int vertex<T>::get_weight(size_t idx) const {
                return idx < weights.size() ? weights[idx] : 0;
            }

            template<typename T>
            array<vertex<T> *> vertex<T>::get_edges() const{
                return edges;
            }
        }
    }
}
