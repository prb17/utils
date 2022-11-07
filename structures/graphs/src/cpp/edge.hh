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
                    edge(T*);                    
                    edge(edge const &);
                    edge& operator=(edge const &);
                    
                    T* get_vertex() const;
                    virtual std::string to_string() const;
            };
            
            template<typename T>
            edge<T>::edge(T* ptr) : v{ptr} {}
            
            template<typename T>
            edge<T>::edge(edge const &e) {
                v = e.v;
            }

            template<typename T>
            edge<T>& edge<T>::operator=(edge const &e) {
                if (&e != this) {
                    v = e.v;
                }
                return *this;
            }
            
            template<typename T>
            T* edge<T>::get_vertex() const { return v; }

            template<typename T>
            std::string edge<T>::to_string() const {
                std::stringstream stream;
                stream <<  "(" << v->get_id() << ")";
                return stream.str();
            }

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
                    weighted_edge(T*, int);
                    
                    virtual std::string to_string() const override;
            };
            
            template<typename T>
            weighted_edge<T>::weighted_edge(T* ptr, int w ) : edge<T>(ptr), weight{w} {}
            
            template<typename T>
            std::string weighted_edge<T>::to_string() const {
                std::stringstream stream;
                stream <<  "(" << this->get_vertex()->get_id() << ", " << weight << ")";
                return stream.str();
            }

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