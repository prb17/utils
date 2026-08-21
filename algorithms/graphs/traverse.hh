#pragma once

#include <string>
#include <sstream>

#include "graph.hh"
#include "vertex.hh"
#include "array.hh"
#include "queue.hh"

#include "search.hh"

namespace prb17 {
    namespace utils {
        namespace algorithms {
            namespace graphs {

                using prb17::utils::structures::graph;
                using prb17::utils::structures::vertex;
                using prb17::utils::structures::array;
                using prb17::utils::structures::queue;

                /**
                 * @brief recursive preorder helper for dfs.
                 *
                 * Visited-membership is intentionally checked with
                 * prb17::utils::algorithms::search::find so the search algorithm
                 * stays swappable and does not live on the container.
                 */
                template<typename T>
                void dfs_visit(vertex<T>* node, array<vertex<T>*>& visited, array<vertex<T>*>& result) {
                    if (node == nullptr) { return; }
                    if (search::find(visited, node) != -1) { return; }

                    visited.add(node);
                    result.add(node);

                    for (size_t i=0; i<node->num_edges(); i++) {
                        vertex<T>* neighbor = node->get_connected_vertex(i);
                        if (search::find(visited, neighbor) == -1) {
                            dfs_visit(neighbor, visited, result);
                        }
                    }
                }

                /**
                 * @brief depth first traversal in preorder starting from start_id.
                 *
                 * @return array of vertices in the order they were first visited;
                 *      an empty array when start_id is not present in the graph.
                 */
                template<typename T>
                array<vertex<T>*> dfs(const graph<T>& g, std::string start_id) {
                    array<vertex<T>*> result{};
                    array<vertex<T>*> visited{};

                    vertex<T>* start = g.get(start_id);
                    if (start == nullptr) { return result; }

                    dfs_visit(start, visited, result);
                    return result;
                }

                /**
                 * @brief breadth first traversal in level order starting from start_id.
                 *      Uses a structures::queue as the frontier.
                 *
                 * @return array of vertices in level order;
                 *      an empty array when start_id is not present in the graph.
                 */
                template<typename T>
                array<vertex<T>*> bfs(const graph<T>& g, std::string start_id) {
                    array<vertex<T>*> result{};
                    array<vertex<T>*> visited{};

                    vertex<T>* start = g.get(start_id);
                    if (start == nullptr) { return result; }

                    queue<vertex<T>*> frontier{};
                    frontier.enqueue(start);
                    visited.add(start);

                    while(!frontier.empty()) {
                        vertex<T>* node = frontier.dequeue();
                        result.add(node);

                        for (size_t i=0; i<node->num_edges(); i++) {
                            vertex<T>* neighbor = node->get_connected_vertex(i);
                            if (search::find(visited, neighbor) == -1) {
                                visited.add(neighbor);
                                frontier.enqueue(neighbor);
                            }
                        }
                    }
                    return result;
                }

                /**
                 * @brief renders every vertex and its neighbors as a string.
                 *
                 * Iterates every vertex via graph::at() and starts a fresh dfs from
                 * each still-unvisited vertex, so disconnected components are all
                 * included (not just the component reachable from vertex 0).
                 */
                template<typename T>
                std::string to_string(const graph<T>& g) {
                    std::stringstream stream;
                    array<vertex<T>*> visited{};

                    for (size_t i=0; i<g.get_count(); i++) {
                        vertex<T>* root = g.at(i);
                        if (root == nullptr) { continue; }
                        if (search::find(visited, root) != -1) { continue; }

                        array<vertex<T>*> component = dfs(g, root->get_id());
                        for (size_t k=0; k<component.size(); k++) {
                            vertex<T>* node = component[k];
                            if (search::find(visited, node) != -1) { continue; }
                            visited.add(node);

                            stream << "Node: " << node->get_id() << " | Neighbors: ";
                            for (size_t j=0; j<node->num_edges(); j++) {
                                vertex<T>* neighbor = node->get_connected_vertex(j);
                                if (neighbor != nullptr) {
                                    stream << neighbor->get_id() << " ";
                                }
                            }
                            stream << std::endl;
                        }
                    }
                    return stream.str();
                }

                /**
                 * @brief renders the graph as an adjacency list, annotating each
                 *      edge with its weight.
                 */
                template<typename T>
                std::string to_adjacency_list(const graph<T>& g) {
                    std::stringstream stream;
                    for (size_t i=0; i<g.get_count(); i++) {
                        vertex<T>* v = g.at(i);
                        if (v == nullptr) { continue; }

                        stream << "Node: " << v->get_id() << " | ";
                        for (size_t j=0; j<v->num_edges(); j++) {
                            vertex<T>* neighbor = v->get_connected_vertex(j);
                            if (neighbor != nullptr) {
                                stream << neighbor->get_id() << "(w:" << v->get_weight(j) << ") ";
                            }
                        }
                        stream << std::endl;
                    }
                    return stream.str();
                }

            }
        }
    }
}
