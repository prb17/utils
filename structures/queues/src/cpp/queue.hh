#pragma once
#include "stack.hh"

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class queue : public stack<T> {
                private:
                    //constructors
                    //accessors
                    //modifier
                    virtual T pop() override;
                    //overloaded operators   

                public:
                    //constructors
                    queue();
                    queue(size_t);
                    ~queue();

                    //accessors
                    T front();
                    T back();

                    //modifiers
                    void enqueue(T);
                    T dequeue();
                    
                    //overloaded operators   
            };

            //constructors
            template<typename T>
            queue<T>::queue() : queue(0) {}

            template<typename T>
            queue<T>::queue(size_t size) : stack<T>(size) {}

            template<typename T>
            queue<T>::~queue() {}

            //accessors
            template<typename T>
            T queue<T>::front() {
                return (this->data[0]).value();
            }

            template<typename T>
            T queue<T>::back() {
                return this->peek();
            }

            template<typename T>
            T queue<T>::pop() {
              stack<T> tmp_stack{};
              while(!this->empty()) {
                tmp_stack.push(this->stack<T>::pop());
              }

              T retval = tmp_stack.pop();

              while(!tmp_stack.empty()) {
                this->enqueue(tmp_stack.pop());
              }
              return retval;
            }

            //modifiers
            template<typename T>
            void queue<T>::enqueue(T val) {
                this->push(val);
            }

            template<typename T>
            T queue<T>::dequeue() {
                return pop();
            }
            
            //overloaded operators   
        }
    }
}