#pragma once
#include "stack.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            template<typename T>
            class queue : public stack<T> {
                private:
                    //constructors
                    //accessors
                    
                    //modifier
                    virtual T pop() override;
                    virtual void push(T) override;
                    //overloaded operators   

                public:
                    //constructors
                    queue();
                    queue(size_t);
                    queue(const array<T> &);
                    queue(const queue<T> &);
                    ~queue();

                    //accessors
                    T front();
                    T back();

                    //modifiers
                    void enqueue(T);
                    T dequeue();
                    
                    //overloaded operators   
                    queue& operator=(queue &);
                    queue operator=(queue);
            };

            //constructors
            template<typename T>
            queue<T>::queue() : queue(0) {}

            template<typename T>
            queue<T>::queue(size_t size) : stack<T>(size) {}

            template<typename T>
            queue<T>::queue(const array<T> &a) : stack<T>(a) {}

            template<typename T>
            queue<T>::queue(const queue<T> &q) : stack<T>(q) {}

            template<typename T>
            queue<T>::~queue() {}

            //accessors
            template<typename T>
            T queue<T>::front() {
                return (this->data[0])->value();
            }

            template<typename T>
            T queue<T>::back() {
                return this->peek();
            }

            template<typename T>
            void queue<T>::push(T val) {
              this->stack<T>::push(val);
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
            template<typename T>
            queue<T>& queue<T>::operator=(queue &q) {
                if (&q != this) {
                    this = q;
                }
                return *this;
            }

            template<typename T>
            queue<T> queue<T>::operator=(queue q) {
                this->clear();
                for(int i=0; i<q.size(); i++) {
                    this->enqueue(q[i]);
                }
                return *this;
            }
        }
    }
}