#pragma once
#include <ostream>
#include <sstream>
#include <string>

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class edge {
                private:
                    T v;

                public:
                    T get_vertex() { return v; }
                    void set_vertex(T ptr) { v = ptr; }

                    edge() : edge(nullptr) {}

                    edge(T ptr) : v{ptr} {}

                    std::string to_string() const {
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
            class weighted_edge : public edge<T> {
                private:
                    int weight;

                public:
                    weighted_edge() {}

                    weighted_edge(int w) : weighted_edge(w, nullptr) {}

                    weighted_edge(int w, T ptr) : edge<T>(ptr), weight{w} {}
            };
        }
    }
}