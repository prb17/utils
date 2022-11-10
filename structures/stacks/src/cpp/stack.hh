#pragma once
#include "array.hh"

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class stack : public array<T> {
                private:

                public:
                    //constructors
                    stack();
                    stack(size_t);
                    stack(T*, size_t);
                    ~stack();

                    //accessors
                    T peek();

                    //modifiers
                    virtual T pop();
                    void push(T);
            };

            //constructors
            template<typename T>
            stack<T>::stack() : stack(0) {}

            template<typename T>
            stack<T>::stack(size_t size) : stack(nullptr, size) {}

            template<typename T>
            stack<T>::stack(T* in_data, size_t size) : array<T>(in_data, size) {}

            template<typename T>
            stack<T>::~stack() {}

            //accessors
            template<typename T>
            T stack<T>::peek() {
                return this->data[this->size() - 1].value();
            }

            //modifiers
            template<typename T>
            T stack<T>::pop() {
                T retval = this->get_value(this->size() - 1);
                this->remove(this->size() - 1);
                return retval;
            }

            template<typename T>
            void stack<T>::push(T val) {
                this->add(val);
            }
        }    
    }
}
