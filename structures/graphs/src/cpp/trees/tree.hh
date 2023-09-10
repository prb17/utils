#pragma once

#include "graph.hh"
#include "vertex.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class tree : public graph<T> {
                private:

                public:
                    tree();
                    ~tree();
            };

            template<typename T>
            tree<T>::tree() : graph<T>() {}

            template<typename T>
            tree<T>::~tree() {}
        }
    }
}