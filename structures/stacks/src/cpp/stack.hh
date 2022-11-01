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
                    void update_tail() {
                        tail = &(this->get_data()[this->get_size() - 1]);
                    }

                public:
                    //constructors
                    stack() : stack(0) {}

                    stack(size_t size) : stack(nullptr, size) {}

                    stack(T* in_data, size_t size) : array<T>(in_data, size) {
                        if (size > 0 && in_data) {
                            update_tail();
                        }                        
                    }

                    ~stack() {
                        tail = nullptr;
                    }

                    //accessors
                    T peek() {
                        return *tail;
                    }

                    //modifiers
                    void pop() {
                        this->remove(this->get_size() - 1);
                        update_tail();
                    }

                    void push(T val) {
                        this->add(val);
                        update_tail();
                    }
            };

        }    
    }
}
