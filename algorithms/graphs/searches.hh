#pragma once

#include <functional>
#include <string>
#include <cstddef>

#include "graph.hh"
#include "vertex.hh"
#include "array.hh"

#include "search.hh"   // algorithms::search::find (visited membership)
#include "sorts.hh"    // insertion_sort for the graph binary_search convenience

namespace prb17 {
    namespace utils {
        namespace algorithms {
            namespace graphs {

                using prb17::utils::structures::graph;
                using prb17::utils::structures::vertex;
                using prb17::utils::structures::array;

                template<typename T, typename Compare>
                static bool values_equal(const T& a, const T& b, const Compare& comp) {
                    return !comp(a, b) && !comp(b, a);
                }

                template<typename T, typename Compare>
                vertex<T>* dfs_search_helper(vertex<T>* node, const T& target,
                                             const Compare& comp, array<vertex<T>*>& visited) {
                    if (node == nullptr) { return nullptr; }
                    if (search::find(visited, node) != -1) { return nullptr; } // already seen
                    visited.add(node);

                    if (values_equal(node->get(), target, comp)) { return node; }

                    for (size_t i=0; i<node->num_edges(); i++) {
                        vertex<T>* hit = dfs_search_helper(node->get_connected_vertex(i), target, comp, visited);
                        if (hit != nullptr) { return hit; }
                    }
                    return nullptr;
                }

                /**
                 * @brief depth-first search for a value: walks the graph depth-first
                 *      from start_id and returns the first vertex whose value equals
                 *      target, or nullptr if start_id is missing or target is not found.
                 *
                 * This is the general-graph search (any structure, cycles handled via a
                 * visited set) -- distinct from bst::find, which exploits BST ordering.
                 */
                template<typename T, typename Compare = std::less<T>>
                vertex<T>* dfs_search(const graph<T>& g, std::string start_id, T target, Compare comp = Compare{}) {
                    vertex<T>* start = g.get(start_id);
                    if (start == nullptr) { return nullptr; }
                    array<vertex<T>*> visited{};
                    return dfs_search_helper(start, target, comp, visited);
                }

                /**
                 * @brief binary search over an already-sorted sequence: returns the
                 *      index of target, or -1 if absent. O(log n).
                 */
                template<typename T, typename Compare = std::less<T>>
                int binary_search(const array<T>& sorted, T target, Compare comp = Compare{}) {
                    int lo = 0;
                    int hi = (int)sorted.size() - 1;
                    while (lo <= hi) {
                        int mid = lo + (hi - lo) / 2;
                        if (comp(target, sorted[mid])) {
                            hi = mid - 1;
                        } else if (comp(sorted[mid], target)) {
                            lo = mid + 1;
                        } else {
                            return mid;
                        }
                    }
                    return -1;
                }

                /**
                 * @brief binary search of a graph's values for target. A graph has no
                 *      inherent order, so this first sorts the values, then binary
                 *      searches; it returns target's index in that sorted order, or -1.
                 *
                 * The sort makes this O(n^2) + O(log n) -- for repeated ordered lookups
                 * prefer a bst (bst::find) rather than re-sorting a graph each time.
                 */
                template<typename T, typename Compare = std::less<T>>
                int binary_search(const graph<T>& g, T target, Compare comp = Compare{}) {
                    array<T> sorted = insertion_sort(g, comp);
                    return binary_search(sorted, target, comp);
                }
            }
        }
    }
}
