#pragma once
#include "array.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class stack : public array<T> {
                private:
                    //constructors
                    //accessors
                    //modifiers
                    //overloaded operators                    

                public:
                    //constructors
                    stack();
                    stack(size_t);
                    stack(const array<T> &);
                    stack(const stack<T> &);
                    ~stack();

                    //accessors
                    T peek();

                    //modifiers
                    virtual T pop();
                    void push(T);

                    //overloaded operators
                    stack& operator=(stack &);
                    stack operator=(stack);
            };

            //constructors
            template<typename T>
            stack<T>::stack() : stack(0) {}

            template<typename T>
            stack<T>::stack(size_t size) : array<T>(size) {}

            template<typename T>
            stack<T>::stack(const array<T> &a) : array<T>(a) {}

            template<typename T>
            stack<T>::stack(const stack<T> &s) : array<T>(s) {}

            template<typename T>
            stack<T>::~stack() {}

            //accessors
            template<typename T>
            T stack<T>::peek() {
                return (*this)[this->size() - 1];
            }

            //modifiers
            template<typename T>
            T stack<T>::pop() {
                T retval = peek();
                this->remove(this->size() - 1);
                return retval;
            }

            template<typename T>
            void stack<T>::push(T val) {
                this->add(val);
            }
            
            //overloaded operators
            template<typename T>
            stack<T>& stack<T>::operator=(stack &s) {
                if (&s != this) {
                    this = s;
                }
                return *this;
            }

            template<typename T>
            stack<T> stack<T>::operator=(stack s) {
                this->clear();
                for(int i=0; i<s.size(); i++) {
                    this->push(s[i]);
                }
                return *this;
            }
        }    
    }
}
