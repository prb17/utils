#pragma once
#include "array.hh"

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class queue : public array<T> {
                private:
                    T* head;
                    int head_idx;
                    T* tail;

                    //modifiers
                    void update_head();
                    void update_tail();

                public:
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

            //modifiers
            template<typename T>
            void queue<T>::update_head() {
                head = &(this->get_data()[head_idx]);
            }

            template<typename T>
            void queue<T>::update_tail() {
                tail = &(this->get_data()[this->get_size() - 1]);
            }

            template<typename T>
            queue<T>::queue() : queue(0) {}

            template<typename T>
            queue<T>::queue(size_t size) : array<T>(size) {
                head = tail = nullptr;
            }

            template<typename T>
            queue<T>::~queue() {
                head = nullptr;
                tail = nullptr;
            }

            template<typename T>
            T queue<T>::front() {
                return *head;
            }

            template<typename T>
            T queue<T>::back() {
                return *tail;
            }

            //modifiers
            template<typename T>
            void queue<T>::enqueue(T val) {
                this->add(val);
                update_head();
                update_tail();
            }

            template<typename T>
            void queue<T>::dequeue() {
                head_idx--;
                update_head();
                update_tail();
            }
        }
    }
}