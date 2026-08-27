#pragma once

#include <functional>
#include <cstddef>

#include "array.hh"

namespace prb17 {
    namespace utils {
        namespace structures {

            /**
             * @brief A sequence that keeps its elements in sorted order. Rather than
             *      being a distinct kind of storage, it is an array whose ordering
             *      invariant is maintained on insert: each new element is placed at
             *      its sorted position (found by binary search), so membership and
             *      lookups are O(log n).
             *
             * Compare defines the order; the default std::less sorts ascending.
             * Duplicates are permitted and kept adjacent.
             */
            template<typename T, typename Compare = std::less<T>>
            class sorted_sequence {
                private:
                    array<T> data;
                    Compare comp;

                    // first index whose element is not ordered before `value`
                    // (i.e. the leftmost slot where value could be inserted)
                    size_t lower_bound(const T& value) const {
                        size_t lo = 0;
                        size_t hi = data.size();
                        while (lo < hi) {
                            size_t mid = lo + (hi - lo) / 2;
                            if (comp(data[mid], value)) {
                                lo = mid + 1;
                            } else {
                                hi = mid;
                            }
                        }
                        return lo;
                    }

                    bool equal(const T& a, const T& b) const {
                        return !comp(a, b) && !comp(b, a);
                    }

                public:
                    sorted_sequence() : data{}, comp{} {}
                    sorted_sequence(Compare c) : data{}, comp{c} {}

                    //modifiers
                    void insert(T value) {
                        data.insert(lower_bound(value), value);
                    }

                    /**
                     * @brief removes one occurrence of value. Returns false if absent.
                     */
                    bool remove(T value) {
                        int idx = find(value);
                        if (idx < 0) { return false; }
                        data.remove((size_t)idx);
                        return true;
                    }

                    void clear() { data.clear(); }

                    //accessors
                    /**
                     * @brief index of value via binary search, or -1 if not present.
                     */
                    int find(const T& value) const {
                        size_t pos = lower_bound(value);
                        if (pos < data.size() && equal(data[pos], value)) {
                            return (int)pos;
                        }
                        return -1;
                    }

                    bool contains(const T& value) const { return find(value) != -1; }

                    const T& operator[](size_t idx) const { return data[idx]; }
                    const T& min() const { return data[0]; }
                    const T& max() const { return data[data.size() - 1]; }
                    size_t size() const { return data.size(); }
                    bool empty() const { return data.size() == 0; }
            };
        }
    }
}
