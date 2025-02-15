#pragma once

#include "graph.hh"
#include "vertex.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class tree : public graph<T> {
                private:
                    uint8_t max_leaf_nodes; //depicts what type of tree i.e. binary, trinary, etc.

                public:
                    tree(uint8_t);
                    ~tree();


            };

            template<typename T>
            tree<T>::tree(uint8_t n) : max_leaf_nodes{n}, graph<T>() {}

            template<typename T>
            tree<T>::~tree() {}

        }
    }
}