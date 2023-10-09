#pragma once

#include "graph.hh"
#include "graph_builder.hh"
#include "concrete_builder_role.hh"

namespace prb17 {
    namespace utils {
        namespace structures {
            template<typename T>
            class sim_vertex {
                private:
                    std::string id;
                    T value;
                    array<std::string> edges;

                protected:

                public:
                    std::string get_id();
                    T get_value();
                    array<std::string> get_edges();

                    sim_vertex(std::string, T, array<std::string>);
            };
            template<typename T>
            sim_vertex<T>::sim_vertex(std::string id, T value, array<std::string> edges) : id{id}, value{value}, edges{edges} {}

            template<typename T>
            std::string sim_vertex<T>::get_id() {
                return id;
            }

            template<typename T>
            T sim_vertex<T>::get_value() {
                return value;
            }

            template<typename T>
            array<std::string> sim_vertex<T>::get_edges() {
                return edges;
            }

            //////////////////////////////////////////////////////
            template<typename T>
            class graph_concrete_builder : public prb17::utils::patterns::creational::builder::concrete_builder_role, public graph_builder {
                private:
                    graph<T>* graph_product();
                    array<sim_vertex<T>*> vertices_to_build;

                protected:

                public:
                    void build() override;
                    void add(std::string, T, array<std::string>);

                    graph_concrete_builder<T>* add(vertex<T> *);
                    graph_concrete_builder<T>* add(std::string, T);
                    graph_concrete_builder<T>* add_edge_to_vertex(std::string, std::string);

                    graph_concrete_builder();
                    ~graph_concrete_builder();
            };

            template<typename T>
            graph_concrete_builder<T>::graph_concrete_builder() : vertices_to_build{} {
                this->product = new graph<T>{};
            }

            template<typename T>
            graph_concrete_builder<T>::~graph_concrete_builder() {
                for(int i=0; i<vertices_to_build.size(); i++) {
                    delete vertices_to_build[i];
                }
            }

            template<typename T>
            graph<T>* graph_concrete_builder<T>::graph_product() {
                return ((graph<T> *)this->product);
            }

            template<typename T>
            void graph_concrete_builder<T>::add(std::string id, T value, array<std::string> edges) {
                sim_vertex<T> *v = new sim_vertex<T>{id, value, edges};
                vertices_to_build.add(v);
            }

            template<typename T>
            graph_concrete_builder<T>* graph_concrete_builder<T>::add(vertex<T> *v) {
                graph_product()->add_vertex(v);
                return this;
            }

            template<typename T>
            graph_concrete_builder<T>* graph_concrete_builder<T>::add(std::string id, T value) {
                vertex<T> *v = new vertex{id, value};
                return add(v);
            }

            template<typename T>
            graph_concrete_builder<T>* graph_concrete_builder<T>::add_edge_to_vertex(std::string id, std::string edge_id) {
                vertex<T>* v = graph_product()->get_vertex(id);
                vertex<T>* v2 = graph_product()->get_vertex(edge_id);
                if (v != nullptr) {
                    v->add_edge(v2);
                }
                return this;
            }

            template<typename T>
            void graph_concrete_builder<T>::build() {
                //create the nodes
                for(int i=0; i<vertices_to_build.size(); i++) {
                    add(vertices_to_build[i]->get_id(), vertices_to_build[i]->get_value());
                }
                
                //create the edges
                for(int i=0; i<vertices_to_build.size(); i++) {
                    vertex<T> *v = graph_product()->get_vertex(vertices_to_build[i]->get_id());
                    for(int j=0; j<vertices_to_build[i]->get_edges().size(); j++) {
                        add_edge_to_vertex(v->get_id(), vertices_to_build[i]->get_edges()[j]);
                    }
                }
            }
        }
    }
}