#pragma once

#include "tree.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            /**
             * @brief A binary tree is just a tree whose branching factor is 2:
             *      every node has at most two children.
             *
             * All storage/access behavior is inherited from tree<T>; children are
             * added left-to-right via add_child (index 0 is the "left" child,
             * index 1 the "right").
             */
            template<typename T>
            class binary_tree : public tree<T> {
                public:
                    binary_tree() : tree<T>(2) {}
            };
        }
    }
}
