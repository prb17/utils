#pragma once
#include "array.hh"
#include <string>

namespace pbrady {
    namespace structures {

        template<typename T>
        class stack : public array<T> {
            private:
                T *tail;

            public:
                //constructors
                stack() {
                    tail = nullptr;
                }

                stack(T* in_data, size_t size) : array<T>(in_data, size) {
                    tail = &(this->get_data()[this->get_size() - 1]);
                }

                ~stack() {
                    tail = nullptr;
                }

                //accessors
                T peek() {
                    return *tail;
                }

                //modifiers
                T pop() {

                }

                void push() {
                    
                }
        };

    }    
}
