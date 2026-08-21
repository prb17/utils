#include <iostream>
#include <string>

#include "validator.hh"
#include "logger.hh"

#include "tree.hh"
#include "binary_tree.hh"
#include "tree_traverse.hh"

using namespace prb17::utils::structures;
namespace algo = prb17::utils::algorithms::graphs;

static prb17::utils::logger logger{"tree_test"};

template<typename T>
static array<std::string> ids_of(array<vertex<T>*> nodes) {
    array<std::string> ids{};
    for (size_t i=0; i<nodes.size(); i++) {
        ids.add(nodes[i]->get_id());
    }
    return ids;
}

// Build a tree from a config block of the form:
//   { "max_children": N,
//     "root":  {"id": .., "value": ..},
//     "nodes": [ {"id": .., "value": .., "parent": ..}, ... ] }
// nodes must be listed parent-before-child.
template<typename T>
static tree<T>* build_tree_from_config(prb17::utils::parsers::json_parser jp) {
    size_t mc = (size_t)jp.as_int("max_children");
    tree<T>* t = new tree<T>{mc};

    prb17::utils::parsers::json_parser root{jp.get_json_value()["root"]};
    t->add_root(root.as_string("id"), root.as_value<T>("value"));

    for (Json::Value::ArrayIndex i=0; i < jp.get_json_value()["nodes"].size(); i++) {
        prb17::utils::parsers::json_parser n{jp.get_json_value()["nodes"][i]};
        t->add_child(n.as_string("parent"), n.as_string("id"), n.as_value<T>("value"));
    }
    return t;
}

// the tree stores the expected number of nodes
template<typename T>
bool testTreeNodeCount(prb17::utils::parsers::json_parser jp) {
    tree<T>* t = build_tree_from_config<T>(jp);
    int expected = jp.as_int("expected_count");
    bool result = (int)t->get_count() == expected;
    logger.debug("tree count: '{}', expected: '{}'", t->get_count(), expected);
    t->cleanup();
    delete t;
    return result;
}

// get_root returns the configured root
template<typename T>
bool testRootAccessor(prb17::utils::parsers::json_parser jp) {
    tree<T>* t = build_tree_from_config<T>(jp);
    std::string expected = jp.as_string("expected_root");
    vertex<T>* root = t->get_root();
    bool result = (root != nullptr) && (root->get_id() == expected);
    t->cleanup();
    delete t;
    return result;
}

// get_children returns a parent's children in insertion (left-to-right) order
template<typename T>
bool testGetChildren(prb17::utils::parsers::json_parser jp) {
    tree<T>* t = build_tree_from_config<T>(jp);
    std::string parent = jp.as_string("children_of");
    array<std::string> got = ids_of(t->get_children(parent));
    array<std::string> expected = jp.as_string_array("expected_children");
    logger.debug("children of '{}': {}, expected: {}", parent, got, expected);
    t->cleanup();
    delete t;
    return got == expected;
}

// a full parent rejects further children; count is unchanged and is_full reports true
template<typename T>
bool testMaxChildrenEnforced(prb17::utils::parsers::json_parser jp) {
    tree<T>* t = build_tree_from_config<T>(jp);
    std::string full_parent = jp.as_string("full_parent");

    size_t before = t->get_count();
    bool added = t->add_child(full_parent, "__overflow__", jp.as_value<T>("overflow_value"));
    size_t after = t->get_count();

    bool result = (added == false) && (before == after) && t->is_full(full_parent);
    logger.debug("overflow add returned '{}', count before '{}' after '{}', is_full '{}'",
                 added, before, after, t->is_full(full_parent));
    t->cleanup();
    delete t;
    return result;
}

// preorder: root, then subtrees left-to-right
template<typename T>
bool testPreorder(prb17::utils::parsers::json_parser jp) {
    tree<T>* t = build_tree_from_config<T>(jp);
    array<std::string> got = ids_of(algo::preorder(*t));
    array<std::string> expected = jp.as_string_array("expected_preorder");
    logger.debug("preorder: {}, expected: {}", got, expected);
    t->cleanup();
    delete t;
    return got == expected;
}

// postorder: subtrees left-to-right, then the node
template<typename T>
bool testPostorder(prb17::utils::parsers::json_parser jp) {
    tree<T>* t = build_tree_from_config<T>(jp);
    array<std::string> got = ids_of(algo::postorder(*t));
    array<std::string> expected = jp.as_string_array("expected_postorder");
    logger.debug("postorder: {}, expected: {}", got, expected);
    t->cleanup();
    delete t;
    return got == expected;
}

// level order: depth by depth
template<typename T>
bool testLevelOrder(prb17::utils::parsers::json_parser jp) {
    tree<T>* t = build_tree_from_config<T>(jp);
    array<std::string> got = ids_of(algo::level_order(*t));
    array<std::string> expected = jp.as_string_array("expected_levelorder");
    logger.debug("level order: {}, expected: {}", got, expected);
    t->cleanup();
    delete t;
    return got == expected;
}

// height: edges on the longest root-to-leaf path
template<typename T>
bool testHeight(prb17::utils::parsers::json_parser jp) {
    tree<T>* t = build_tree_from_config<T>(jp);
    int expected = jp.as_int("expected_height");
    int got = algo::height(*t);
    logger.debug("height: '{}', expected: '{}'", got, expected);
    t->cleanup();
    delete t;
    return got == expected;
}

// a binary_tree is a tree with max_children == 2: two children fit, a third does not
template<typename T>
bool testBinaryTreeClass(prb17::utils::parsers::json_parser) {
    binary_tree<T> bt{};
    bool result = bt.max_children() == 2;

    result &= bt.add_root("root", T{});
    result &= bt.add_child("root", "left", T{});
    result &= bt.add_child("root", "right", T{});
    result &= (bt.add_child("root", "third", T{}) == false); // full at 2
    result &= bt.is_full("root");
    result &= bt.get_count() == 3;

    logger.debug("binary_tree enforced 2-children cap: '{}'", result);
    bt.cleanup();
    return result;
}

// the default tree is unbounded (max_children == 0): any number of children fit
template<typename T>
bool testUnboundedDefault(prb17::utils::parsers::json_parser) {
    tree<T> t{}; // default: unbounded
    bool result = t.max_children() == 0;

    result &= t.add_root("root", T{});
    for (int i=0; i<5; i++) {
        result &= t.add_child("root", std::string("c") + std::to_string(i), T{});
    }
    result &= (t.is_full("root") == false);
    result &= t.get_count() == 6;

    logger.debug("unbounded tree accepted all children: '{}'", result);
    t.cleanup();
    return result;
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;

    tests.add(prb17::utils::test{"testTreeNodeCount", &testTreeNodeCount<T>});
    tests.add(prb17::utils::test{"testRootAccessor", &testRootAccessor<T>});
    tests.add(prb17::utils::test{"testGetChildren", &testGetChildren<T>});
    tests.add(prb17::utils::test{"testMaxChildrenEnforced", &testMaxChildrenEnforced<T>});
    tests.add(prb17::utils::test{"testPreorder", &testPreorder<T>});
    tests.add(prb17::utils::test{"testPostorder", &testPostorder<T>});
    tests.add(prb17::utils::test{"testLevelOrder", &testLevelOrder<T>});
    tests.add(prb17::utils::test{"testHeight", &testHeight<T>});
    tests.add(prb17::utils::test{"testBinaryTreeClass", &testBinaryTreeClass<T>});
    tests.add(prb17::utils::test{"testUnboundedDefault", &testUnboundedDefault<T>});

    return tests;
}
template<typename T>
static prb17::utils::structures::array<prb17::utils::test> tree_tests = build_tests<T>();

#define MIN_NUM_ARGS 2
int main(int argc, char** argv) {
    if (argc < MIN_NUM_ARGS) {
        throw prb17::utils::exception("This test requires a config file to be provided");
    }
    prb17::utils::structures::array<std::string> test_files{};
    for (int i=1; i<argc; i++) {
        //TODO: test if string is a valid file name/path
        logger.debug("adding test file to validator: '{}'", &argv[i][0]);
        test_files.add(&argv[i][0]);
    }
    prb17::utils::validator validator{test_files};

    validator.add_tests(tree_tests<int>);

    logger.info("Starting validation tests of tree_tests");
    validator.validate();
    logger.info("Finished validation tests of tree_tests");
}
