#pragma once

#include <functional>
#include <cstddef>

#include "heap.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            /**
             * @brief A priority queue: a queue whose "front" is always the
             *      highest-priority element rather than the oldest. It is a thin
             *      adaptor over heap<T, Compare> — every enqueue/dequeue routes
             *      through the heap, which maintains the ordering invariant.
             *
             * The default std::less makes the smallest element the highest priority
             * (a min priority queue); pass std::greater<T> for a max priority queue.
             */
            template<typename T, typename Compare = std::less<T>>
            class priority_queue {
                private:
                    heap<T, Compare> h;

                public:
                    priority_queue() : h{} {}
                    priority_queue(Compare c) : h{c} {}

                    //modifiers
                    void enqueue(T value) { h.push(value); }
                    T dequeue() { return h.pop(); }

                    //accessors
                    T peek() const { return h.top(); }
                    size_t size() const { return h.size(); }
                    bool empty() const { return h.empty(); }
                    void clear() { h.clear(); }
            };
        }
    }
}
