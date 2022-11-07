#pragma once
#include "array.hh"

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class stack : public array<T> {
                private:
                    T *tail;

                    //modifiers
                    void update_tail();

                public:
                    //constructors
                    stack();
                    stack(size_t);
                    stack(T*, size_t);
                    ~stack();

                    //accessors
                    T peek();

                    //modifiers
                    void pop();
                    void push(T);
            };

            //modifiers
            template<typename T>
            void stack<T>::update_tail() {
                tail = &(this->get_data()[this->get_size() - 1]);
            }

            //constructors
            template<typename T>
            stack<T>::stack() : stack(0) {}

            template<typename T>
            stack<T>::stack(size_t size) : stack(nullptr, size) {}

            template<typename T>
            stack<T>::stack(T* in_data, size_t size) : array<T>(in_data, size) {
                if (size > 0 && in_data) {
                    update_tail();
                }                        
            }

            template<typename T>
            stack<T>::~stack() {
                tail = nullptr;
            }

            //accessors
            template<typename T>
            T stack<T>::peek() {
                return *tail;
            }

            //modifiers
            template<typename T>
            void stack<T>::pop() {
                this->remove(this->get_size() - 1);
                update_tail();
            }

            template<typename T>
            void stack<T>::push(T val) {
                this->add(val);
                update_tail();
            }
        }    
    }
}
