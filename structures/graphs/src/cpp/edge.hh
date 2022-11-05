#pragma once
#include <ostream>
#include <sstream>
#include <string>
#include "vertex.hh"

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class edge {
                private:
                    T* v;

                public:
                    edge() = delete;
                    edge(T* ptr) : v{ptr} {}
                    
                    T* get_vertex() const{ return v; }
                    // void set_vertex(T ptr) { v = ptr; }

                    virtual std::string to_string() const {
                        std::stringstream stream;
                        stream <<  "(" << v->get_id() << ")";
                        return stream.str();
                    }
            };

            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const edge<T>& e) {
                return stream << e.to_string();
            }

            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const edge<T>* e) {
                return stream << e->to_string();
            }

            template<typename T>
            class weighted_edge : public edge<T> {
                private:
                    int weight;

                public:
                    weighted_edge() = delete;
                    weighted_edge(int w, T* ptr) : edge<T>(ptr), weight{w} {}
                    
                    virtual std::string to_string() const override {
                        std::stringstream stream;
                        stream <<  "(" << this->get_vertex()->get_id() << ", " << weight << ")";
                        return stream.str();
                    }
            };
            

            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const weighted_edge<T>& we) {
                return stream << we.to_string();
            }

            template<typename T>
            inline std::ostream& operator<<(std::ostream &stream, const weighted_edge<T>* we) {
                return stream << we->to_string();
            }

        }
    }
}