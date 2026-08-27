#pragma once

#include <functional>
#include <utility>
#include <cstddef>

#include "array.hh"
#include "exception.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            /**
             * @brief A binary heap: a complete binary tree, stored implicitly in a
             *      contiguous array. Because the tree is complete there are no gaps,
             *      so the parent/child links are computed from the index rather than
             *      stored:
             *          left(i)  = 2i + 1
             *          right(i) = 2i + 2
             *          parent(i)= (i - 1) / 2
             *
             * The heap owns storage/access and maintains the heap-order invariant
             * itself (via sift-up/sift-down on each push/pop), the same way stack and
             * queue own their LIFO/FIFO invariants here.
             *
             * Compare defines priority: the element x for which Compare(x, y) holds
             * against every other y bubbles to the top. The default std::less yields
             * a min-heap (top() is the minimum).
             */
            template<typename T, typename Compare = std::less<T>>
            class heap {
                private:
                    array<T> data;
                    Compare comp;

                    static size_t parent(size_t i) { return (i - 1) / 2; }
                    static size_t left(size_t i)   { return 2 * i + 1; }
                    static size_t right(size_t i)  { return 2 * i + 2; }

                    void sift_up(size_t i) {
                        while (i > 0 && comp(data[i], data[parent(i)])) {
                            std::swap(data[i], data[parent(i)]);
                            i = parent(i);
                        }
                    }

                    void sift_down(size_t i) {
                        size_t n = data.size();
                        while (true) {
                            size_t top = i;
                            size_t l = left(i);
                            size_t r = right(i);
                            if (l < n && comp(data[l], data[top])) { top = l; }
                            if (r < n && comp(data[r], data[top])) { top = r; }
                            if (top == i) { break; }
                            std::swap(data[i], data[top]);
                            i = top;
                        }
                    }

                public:
                    heap() : data{}, comp{} {}
                    heap(Compare c) : data{}, comp{c} {}

                    //modifiers
                    void push(T value) {
                        data.add(value);
                        sift_up(data.size() - 1);
                    }

                    /**
                     * @brief removes and returns the top (highest-priority) element.
                     * @throw exception when the heap is empty.
                     */
                    T pop() {
                        if (empty()) { throw utils::exception("pop from empty heap"); }
                        T root = data[0];
                        size_t last = data.size() - 1;
                        data[0] = data[last];
                        data.remove(last);
                        if (!empty()) { sift_down(0); }
                        return root;
                    }

                    //accessors
                    T top() const {
                        if (empty()) { throw utils::exception("top of empty heap"); }
                        return data[0];
                    }

                    size_t size() const { return data.size(); }
                    bool empty() const { return data.size() == 0; }
                    void clear() { data.clear(); }
            };
        }
    }
}
