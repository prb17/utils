#pragma once

#include "tree.hh"
#include "graph.hh"
#include "vertex.hh"
#include "array.hh"

#include "traverse.hh"

namespace prb17 {
    namespace utils {
        namespace algorithms {
            namespace graphs {

                using prb17::utils::structures::tree;
                using prb17::utils::structures::vertex;
                using prb17::utils::structures::array;

                /**
                 * @brief preorder: visit the root, then each subtree left-to-right.
                 *      For a tree this is exactly a depth-first preorder, so it
                 *      reuses the graph dfs over the tree's underlying graph.
                 */
                template<typename T>
                array<vertex<T>*> preorder(const tree<T>& t) {
                    vertex<T>* root = t.get_root();
                    if (root == nullptr) { return array<vertex<T>*>{}; }
                    return dfs(t.as_graph(), root->get_id());
                }

                /**
                 * @brief level order (breadth-first): visit nodes depth by depth.
                 *      Reuses the graph bfs over the tree's underlying graph.
                 */
                template<typename T>
                array<vertex<T>*> level_order(const tree<T>& t) {
                    vertex<T>* root = t.get_root();
                    if (root == nullptr) { return array<vertex<T>*>{}; }
                    return bfs(t.as_graph(), root->get_id());
                }

                template<typename T>
                void postorder_visit(const tree<T>& t, vertex<T>* node, array<vertex<T>*>& result) {
                    if (node == nullptr) { return; }
                    array<vertex<T>*> children = t.get_children(node->get_id());
                    for (size_t i=0; i<children.size(); i++) {
                        postorder_visit(t, children[i], result);
                    }
                    result.add(node);
                }

                /**
                 * @brief postorder: visit each subtree left-to-right, then the node
                 *      itself. This is tree-specific (child order matters), so it is
                 *      implemented directly rather than via the generic graph dfs.
                 */
                template<typename T>
                array<vertex<T>*> postorder(const tree<T>& t) {
                    array<vertex<T>*> result{};
                    postorder_visit(t, t.get_root(), result);
                    return result;
                }

                template<typename T>
                int height_at(const tree<T>& t, vertex<T>* node) {
                    if (node == nullptr) { return -1; }
                    array<vertex<T>*> children = t.get_children(node->get_id());
                    int best = -1;
                    for (size_t i=0; i<children.size(); i++) {
                        int h = height_at(t, children[i]);
                        if (h > best) { best = h; }
                    }
                    return best + 1;
                }

                /**
                 * @brief height: number of edges on the longest root-to-leaf path.
                 *      An empty tree is -1; a single-node tree is 0.
                 */
                template<typename T>
                int height(const tree<T>& t) {
                    return height_at(t, t.get_root());
                }

            }
        }
    }
}
