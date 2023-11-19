#pragma once

#include "graph_builder.hh"

namespace prb17 {
    namespace utils {
        namespace structures {
            template<typename T>
            class weighted_graph_builder : public graph_builder<T> {
                private:
                    std::map<std::string, array<int>> node_edge_weights_blueprint;

                    void build_vertices() override;
                    void connect_edges() override;

                protected:

                public:
                    weighted_graph_builder();
                    ~weighted_graph_builder();

                    weighted_graph_builder<T>& add(std::string, T, array<std::string>, array<int>);
            };

            template<typename T>
            weighted_graph_builder<T>::weighted_graph_builder() : graph_builder<T>(), node_edge_weights_blueprint{} {

            }

            template<typename T>
            weighted_graph_builder<T>::~weighted_graph_builder() {

            }

            template<typename T>
            weighted_graph_builder<T>& weighted_graph_builder<T>::add(std::string id, T value, array<std::string> edges, array<int> weights) {
                graph_builder<T>::add(id, value, edges);
                node_edge_weights_blueprint.emplace(id, weights);
                return *this;
            }

            template<typename T>
            void weighted_graph_builder<T>::build_vertices() {
                //create the weighted vertices of this graph
                for(int i=0; i < this->node_id_blueprint.size(); i++) {
                    vertex<T> *v = new weighted_vertex{this->node_id_blueprint[i], this->node_value_blueprint[i]};
                    this->graph_product()->add(v);
                }
            }

            template<typename T>
            void weighted_graph_builder<T>::connect_edges() {
                //connect the edges of this graph
                for(int i=0; i < this->node_id_blueprint.size(); i++) {
                    weighted_vertex<T> *v = static_cast<weighted_vertex<T>*>(this->graph_product()->get(this->node_id_blueprint[i]));
                    for(int j=0; j < this->node_edges_blueprint.at(v->get_id()).size(); j++) {
                        weighted_vertex<T>* v2 = static_cast<weighted_vertex<T>*>(this->graph_product()->get(this->node_edges_blueprint.at(v->get_id())[j]));
                        int weight = this->node_edge_weights_blueprint.at(v->get_id())[j];
                        v->add_edge(v2, weight);
                    }
                }
            }
        }
    }
}