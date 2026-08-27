#pragma once

#include <cstddef>

#include "array.hh"
#include "exception.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            /**
             * @brief A fixed-capacity circular (ring) buffer.
             *
             * Unlike the heap or sorted sequence, this is not an invariant imposed on
             * a generic array — it is a distinct storage discipline: a fixed backing
             * store of `capacity` slots addressed modulo the capacity, with a moving
             * head and a live count. Enqueue/dequeue at either end are O(1) with no
             * shifting, and the indices wrap around the end of the store.
             *
             * The backing store is a plain array pre-filled to `capacity`; the logical
             * contents are tracked separately by head + count.
             */
            template<typename T>
            class ring_buffer {
                private:
                    array<T> data;
                    size_t cap;
                    size_t head;
                    size_t count;

                    size_t index(size_t logical) const { return (head + logical) % cap; }

                public:
                    ring_buffer(size_t capacity) : data{}, cap{capacity}, head{0}, count{0} {
                        for (size_t i = 0; i < cap; i++) { data.add(T{}); }
                    }

                    //modifiers
                    /**
                     * @brief appends value at the tail. Returns false when full.
                     */
                    bool push(T value) {
                        if (full()) { return false; }
                        data[index(count)] = value;
                        count++;
                        return true;
                    }

                    /**
                     * @brief appends value at the tail, overwriting the oldest element
                     *      when the buffer is full (the defining ring-buffer behavior).
                     */
                    void force_push(T value) {
                        if (full()) {
                            data[head] = value;
                            head = (head + 1) % cap;
                        } else {
                            push(value);
                        }
                    }

                    /**
                     * @brief removes and returns the element at the head.
                     * @throw exception when empty.
                     */
                    T pop() {
                        if (empty()) { throw utils::exception("pop from empty ring_buffer"); }
                        T value = data[head];
                        head = (head + 1) % cap;
                        count--;
                        return value;
                    }

                    void clear() { head = 0; count = 0; }

                    //accessors
                    T front() const {
                        if (empty()) { throw utils::exception("front of empty ring_buffer"); }
                        return data[head];
                    }

                    T back() const {
                        if (empty()) { throw utils::exception("back of empty ring_buffer"); }
                        return data[index(count - 1)];
                    }

                    size_t size() const { return count; }
                    size_t capacity() const { return cap; }
                    bool empty() const { return count == 0; }
                    bool full() const { return count == cap; }
            };
        }
    }
}
