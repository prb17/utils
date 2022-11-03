#pragma once
#include "array.hh"

namespace pbrady {
    namespace utils {
        namespace structures {

            template<typename T>
            class queue : public array<T> {
                private:
                    T* head;
                    T* tail;

                    //modifiers
                    void update_head() {
                        head = &(this->get_data()[0]);
                    }
                    void update_tail() {
                        tail = &(this->get_data()[this->get_size() - 1]);
                    }

                public:
                    queue() : queue(0) {}

                    queue(size_t size) : array<T>(size) {
                        head = tail = nullptr;
                    }

                    //accessors
                    T front() {
                        return *head;
                    }

                    T back() {
                        return *tail;
                    }

                    //modifiers
                    void enqueue(T val) {
                        this->add(val);
                        update_head();
                        update_tail();
                    }

                    void dequeue() {
                        this->remove(0);
                        update_head();
                        update_tail();
                    }

            };
        }
    }
}