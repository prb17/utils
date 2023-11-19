#pragma once

#include "graph.hh"
#include "structures_builder.hh"
#include "concrete_builder_role.hh"

#include <utility>

namespace prb17 {
    namespace utils {
        namespace structures {
            template<typename T>
            class graph_builder : public prb17::utils::patterns::creational::builder::concrete_builder_role, public structures_builder {
                private:
                    void build() override;
                    virtual void build_vertices();
                    virtual void connect_edges();

                protected:
                    array<std::string> node_id_blueprint;
                    array<T> node_value_blueprint;
                    std::map<std::string, array<std::string>> node_edges_blueprint;

                public:
                    graph_builder<T>& add(std::string, T);
                    graph_builder<T>& add(std::string, T, array<std::string>);
                    graph_builder<T>& add_edge_to_vertex(std::string, std::string);

                    graph<T>* graph_product();
                    graph_builder();
                    ~graph_builder();
            };
            
            template<typename T>
            graph_builder<T>::graph_builder() : node_id_blueprint{}, node_value_blueprint{}, node_edges_blueprint{} {
                
            }

            template<typename T>
            graph_builder<T>::~graph_builder() {}

            template<typename T>
            graph<T>* graph_builder<T>::graph_product() {
                return static_cast<graph<T>*>(this->product);
            }

            template<typename T>
            graph_builder<T>& graph_builder<T>::add(std::string id, T value, array<std::string> edges) {
                node_id_blueprint.add(id);
                node_value_blueprint.add(value);
                node_edges_blueprint.emplace(id, edges);
                return *this;
            }

            template<typename T>
            graph_builder<T>& graph_builder<T>::add(std::string id, T value) {
                node_id_blueprint.add(id);
                node_value_blueprint.add(value);
                node_edges_blueprint.emplace(id, array<std::string>{});
                return *this;
            }

            template<typename T>
            graph_builder<T>& graph_builder<T>::add_edge_to_vertex(std::string id, std::string edge_id) {
                node_edges_blueprint.at(id).add(edge_id);
                return *this;
            }

            template<typename T>
            void graph_builder<T>::build_vertices() {
                //create the vertices of this graph
                for(int i=0; i<node_id_blueprint.size(); i++) {
                    vertex<T> *v = new vertex{node_id_blueprint[i], node_value_blueprint[i]};
                    graph_product()->add(v);
                }
            }

            template<typename T>
            void graph_builder<T>::connect_edges() {
                //connect the edges of this graph
                for(int i=0; i<node_id_blueprint.size(); i++) {
                    vertex<T> *v = graph_product()->get(node_id_blueprint[i]);
                    for(int j=0; j<node_edges_blueprint.at(v->get_id()).size(); j++) {
                        vertex<T>* v2 = graph_product()->get(node_edges_blueprint.at(v->get_id())[j]);
                        v->add_edge(v2);
                    }
                }
            }

            template<typename T>
            void graph_builder<T>::build() {
                this->product = new graph<T>{};
                this->build_vertices();
                this->connect_edges();
            }
        }
    }
}