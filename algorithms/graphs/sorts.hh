#pragma once

#include <functional>
#include <utility>
#include <cstddef>

#include "graph.hh"
#include "vertex.hh"
#include "array.hh"

namespace prb17 {
    namespace utils {
        namespace algorithms {
            namespace graphs {

                using prb17::utils::structures::graph;
                using prb17::utils::structures::vertex;
                using prb17::utils::structures::array;

                /**
                 * @brief the values held by a graph's vertices, in vertex-insertion
                 *      order (every vertex, regardless of connectivity).
                 *
                 * Sorting is a sequence operation, not a graph one -- these sorts treat
                 * the graph as a bag of values, collect them here, and return them
                 * sorted. Compare defines the order (default std::less = ascending).
                 */
                template<typename T>
                array<T> collect_values(const graph<T>& g) {
                    array<T> values{};
                    for (size_t i=0; i<g.get_count(); i++) {
                        vertex<T>* v = g.at(i);
                        if (v != nullptr) { values.add(v->get()); }
                    }
                    return values;
                }

                /**
                 * @brief bubble sort of the graph's values. Repeatedly swaps adjacent
                 *      out-of-order pairs so the largest "bubbles" to the end each pass.
                 */
                template<typename T, typename Compare = std::less<T>>
                array<T> bubble_sort(const graph<T>& g, Compare comp = Compare{}) {
                    array<T> a = collect_values(g);
                    size_t n = a.size();
                    for (size_t i=0; i<n; i++) {
                        for (size_t j=0; j+1 < n-i; j++) {
                            if (comp(a[j+1], a[j])) {
                                std::swap(a[j], a[j+1]);
                            }
                        }
                    }
                    return a;
                }

                /**
                 * @brief insertion sort of the graph's values. Grows a sorted prefix by
                 *      inserting each next value into its place within it.
                 */
                template<typename T, typename Compare = std::less<T>>
                array<T> insertion_sort(const graph<T>& g, Compare comp = Compare{}) {
                    array<T> a = collect_values(g);
                    for (size_t i=1; i<a.size(); i++) {
                        T key = a[i];
                        size_t j = i;
                        while (j > 0 && comp(key, a[j-1])) {
                            a[j] = a[j-1];
                            j--;
                        }
                        a[j] = key;
                    }
                    return a;
                }

                /**
                 * @brief selection sort of the graph's values. Repeatedly selects the
                 *      smallest remaining value and places it next.
                 */
                template<typename T, typename Compare = std::less<T>>
                array<T> selection_sort(const graph<T>& g, Compare comp = Compare{}) {
                    array<T> a = collect_values(g);
                    size_t n = a.size();
                    for (size_t i=0; i<n; i++) {
                        size_t min = i;
                        for (size_t j=i+1; j<n; j++) {
                            if (comp(a[j], a[min])) { min = j; }
                        }
                        if (min != i) { std::swap(a[i], a[min]); }
                    }
                    return a;
                }
            }
        }
    }
}
