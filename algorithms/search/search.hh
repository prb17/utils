#pragma once

#include "array.hh"
#include "exception.hh"

namespace prb17 {
    namespace utils {
        namespace algorithms {
            namespace search {
                template<typename T>
                int find(const structures::array<T> &arr, T elem) {
                    int idx = 0;
                    while(idx < arr.size()) {
                        if (arr[idx] == elem) {
                            return idx;
                        }
                        idx++;
                    }
                    return -1;
                }

            }
        }
    }
}
