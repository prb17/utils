#pragma once

#include "single_linked_list.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class double_linked_list : public single_linked_list<T> {
                private:
                    vertex<T> *next;
                    vertex<T> *prev;
                    vertex<T> *data;
                public:
                    double_linked_list();
                    ~double_linked_list();
            };

            template<typename T>
            double_linked_list<T>::double_linked_list() : single_linked_list<T>() {}

            template<typename T>
            double_linked_list<T>::~double_linked_list() {}
        }
    }
}