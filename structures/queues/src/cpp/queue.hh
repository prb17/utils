#pragma once
#include "array.hh"

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class queue : public array<T> {
                private:

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
                    void dequeue();
            };

            //constructors
            template<typename T>
            queue<T>::queue() : queue(0) {}

            template<typename T>
            queue<T>::queue(size_t size) : array<T>(size) {}

            template<typename T>
            queue<T>::~queue() {}

            //accessors
            template<typename T>
            T queue<T>::front() {
                return (this->get_data()[0]).value();
            }

            template<typename T>
            T queue<T>::back() {
                return (this->get_data()[array<T>::get_size() - 1]).value();
            }

            //modifiers
            template<typename T>
            void queue<T>::enqueue(T val) {
                this->add(val);
            }

            template<typename T>
            void queue<T>::dequeue() {
                this->remove(0);
            }
        }
    }
}