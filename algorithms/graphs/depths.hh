#pragma once

#include <string>

#include "graph.hh"
#include "vertex.hh"
#include "array.hh"

#include "search.hh"

namespace prb17 {
    namespace utils {
        namespace algorithms {
            namespace graphs {

                using prb17::utils::structures::graph;
                using prb17::utils::structures::vertex;
                using prb17::utils::structures::array;

                /**
                 * @brief A node's depth is its distance, in edges, from a reference
                 *      node -- for a tree with the root as the reference this is the
                 *      usual node depth. Depth is measured by breadth-first traversal
                 *      following edges, so it is the shortest hop count and works for a
                 *      general graph (with cycles) as well as a tree.
                 *
                 * Visited-membership goes through algorithms::search::find, per the
                 * structures-vs-algorithms layering.
                 */

                // build the next BFS level from the current one, marking newly seen
                // nodes visited so each is counted once at its shortest distance
                template<typename T>
                array<vertex<T>*> next_level(const array<vertex<T>*>& current, array<vertex<T>*>& visited) {
                    array<vertex<T>*> next{};
                    for (size_t i=0; i<current.size(); i++) {
                        vertex<T>* node = current[i];
                        for (size_t j=0; j<node->num_edges(); j++) {
                            vertex<T>* neighbor = node->get_connected_vertex(j);
                            if (search::find(visited, neighbor) == -1) {
                                visited.add(neighbor);
                                next.add(neighbor);
                            }
                        }
                    }
                    return next;
                }

                /**
                 * @brief the depth of node_id relative to reference_id: 0 when they are
                 *      the same node, the shortest edge distance otherwise, and -1 when
                 *      either id is missing or node_id is not reachable from reference_id.
                 */
                template<typename T>
                int depth(const graph<T>& g, std::string reference_id, std::string node_id) {
                    vertex<T>* ref = g.get(reference_id);
                    vertex<T>* target = g.get(node_id);
                    if (ref == nullptr || target == nullptr) { return -1; }

                    array<vertex<T>*> visited{};
                    array<vertex<T>*> current{};
                    current.add(ref);
                    visited.add(ref);

                    int d = 0;
                    while (current.size() > 0) {
                        if (search::find(current, target) != -1) { return d; }
                        current = next_level(current, visited);
                        d++;
                    }
                    return -1; // not reachable from the reference
                }

                /**
                 * @brief the sum of the depths of every node reachable from
                 *      reference_id (the reference itself has depth 0). This is the
                 *      "Node Depths" quantity; with a tree's root as the reference it is
                 *      the sum of all node depths. Returns 0 when the reference is
                 *      missing (no nodes to sum).
                 */
                template<typename T>
                int sum_of_depths(const graph<T>& g, std::string reference_id) {
                    vertex<T>* ref = g.get(reference_id);
                    if (ref == nullptr) { return 0; }

                    array<vertex<T>*> visited{};
                    array<vertex<T>*> current{};
                    current.add(ref);
                    visited.add(ref);

                    int d = 0;
                    int sum = 0;
                    while (current.size() > 0) {
                        sum += d * (int)current.size(); // every node in this level is at depth d
                        current = next_level(current, visited);
                        d++;
                    }
                    return sum;
                }
            }
        }
    }
}
