#include <string>

#include "graph.hh"
#include "vertex.hh"
#include "array.hh"
#include "validator.hh"
#include "logger.hh"

#include "bst.hh"

using namespace prb17::utils::structures;
namespace bst = prb17::utils::algorithms::graphs::bst;

static prb17::utils::logger logger{"bst_test"};

// build a BST in a plain graph by inserting the configured values in order
static void build(graph<int>& g, prb17::utils::parsers::json_parser jp) {
    auto values = jp.as_array<int>("values");
    for (size_t i=0; i<values.size(); i++) {
        bst::insert(g, values[i]);
    }
}

// contains finds present values and rejects absent ones
bool testInsertContains(prb17::utils::parsers::json_parser jp) {
    graph<int> g;
    build(g, jp);
    bool result = bst::contains(g, jp.as_int("present"))
               && !bst::contains(g, jp.as_int("absent"));
    g.cleanup();
    return result;
}

// the in-order traversal of the graph, read by the BST algorithm, is sorted
bool testInorderSorted(prb17::utils::parsers::json_parser jp) {
    graph<int> g;
    build(g, jp);
    array<int> got = bst::in_order(g);
    auto expected = jp.as_array<int>("expected");
    logger.debug("in-order: {}, expected: {}", got, expected);
    g.cleanup();
    return got == expected;
}

// find returns the vertex holding the value (or nullptr when absent)
bool testFind(prb17::utils::parsers::json_parser jp) {
    graph<int> g;
    build(g, jp);
    int present = jp.as_int("present");
    vertex<int>* hit = bst::find(g, present);
    bool result = (hit != nullptr) && (hit->get() == present)
               && (bst::find(g, jp.as_int("absent")) == nullptr);
    g.cleanup();
    return result;
}

// a tree built by insert validates as a proper BST
bool testValidateTrue(prb17::utils::parsers::json_parser jp) {
    graph<int> g;
    build(g, jp);
    bool result = bst::validate(g);
    g.cleanup();
    return result;
}

// a hand-built graph that violates the BST property does not validate
bool testValidateFalse(prb17::utils::parsers::json_parser) {
    graph<int> g;
    // root 10 with two children that are both less than it (two "left" children)
    vertex<int>* root = new vertex<int>{"a", 10};
    vertex<int>* c1 = new vertex<int>{"b", 3};
    vertex<int>* c2 = new vertex<int>{"c", 4};
    g.add(root);
    g.add(c1);
    g.add(c2);
    root->add_edge(c1);
    root->add_edge(c2);

    bool result = (bst::validate(g) == false);
    g.cleanup();
    return result;
}

// the canonical "Validate BST" invalid case: a value in the RIGHT subtree that is
// smaller than an ancestor. Every node is locally on a consistent side (15 right
// of 10, 6 left of 15), so only the inherited bound catches it -- 6 sits in 10's
// right subtree yet 6 < 10.
bool testValidateFalseRightSubtree(prb17::utils::parsers::json_parser) {
    graph<int> g;
    vertex<int>* root = new vertex<int>{"a", 10};
    vertex<int>* right = new vertex<int>{"b", 15}; // 15 >= 10 -> root's right child
    vertex<int>* bad = new vertex<int>{"c", 6};    // 6 < 15 -> 15's left child, but 6 < 10
    g.add(root);
    g.add(right);
    g.add(bad);
    root->add_edge(right);
    right->add_edge(bad);

    bool result = (bst::validate(g) == false);
    g.cleanup();
    return result;
}

// mirror image: a value in the LEFT subtree that is >= an ancestor. 12 is a valid
// right child of 5 locally, but it sits in 10's left subtree while 12 >= 10.
bool testValidateFalseLeftSubtree(prb17::utils::parsers::json_parser) {
    graph<int> g;
    vertex<int>* root = new vertex<int>{"a", 10};
    vertex<int>* left = new vertex<int>{"b", 5};   // 5 < 10 -> root's left child
    vertex<int>* bad = new vertex<int>{"c", 12};   // 12 >= 5 -> 5's right child, but 12 >= 10
    g.add(root);
    g.add(left);
    g.add(bad);
    root->add_edge(left);
    left->add_edge(bad);

    bool result = (bst::validate(g) == false);
    g.cleanup();
    return result;
}

// remove covers leaf / one-child / two-child / root / absent (per config); the
// tree stays a valid BST and the value's presence matches expectation
bool testRemove(prb17::utils::parsers::json_parser jp) {
    graph<int> g;
    build(g, jp);
    int value = jp.as_int("remove");

    bst::remove(g, value);

    array<int> got = bst::in_order(g);
    auto expected = jp.as_array<int>("expected");
    bool contains_after = bst::contains(g, value);
    bool expected_contains = jp.as_bool("expected_contains");
    bool still_valid = bst::validate(g);

    logger.debug("after remove {}: {}, expected: {}", value, got, expected);
    g.cleanup();
    return got == expected && contains_after == expected_contains && still_valid;
}

// removing an absent value is a no-op and reports false
bool testRemoveMissing(prb17::utils::parsers::json_parser jp) {
    graph<int> g;
    build(g, jp);
    bool removed = bst::remove(g, jp.as_int("absent"));
    array<int> got = bst::in_order(g);
    auto expected = jp.as_array<int>("expected");
    g.cleanup();
    return (removed == false) && got == expected;
}

#define MIN_NUM_ARGS 2
int main(int argc, char** argv) {
    if (argc < MIN_NUM_ARGS) {
        throw prb17::utils::exception("This test requires a config file to be provided");
    }
    prb17::utils::structures::array<std::string> test_files{};
    for (int i=1; i<argc; i++) {
        test_files.add(&argv[i][0]);
    }
    prb17::utils::validator validator{test_files};

    validator.add_test("testInsertContains", &testInsertContains, "");
    validator.add_test("testInorderSorted", &testInorderSorted, "");
    validator.add_test("testFind", &testFind, "");
    validator.add_test("testValidateTrue", &testValidateTrue, "");
    validator.add_test("testValidateFalse", &testValidateFalse, "");
    validator.add_test("testValidateFalseRightSubtree", &testValidateFalseRightSubtree, "");
    validator.add_test("testValidateFalseLeftSubtree", &testValidateFalseLeftSubtree, "");
    validator.add_test("testRemove", &testRemove, "");
    validator.add_test("testRemoveMissing", &testRemoveMissing, "");

    logger.info("Starting validation tests of bst (graph algorithm) tests");
    validator.validate();
    logger.info("Finished validation tests of bst (graph algorithm) tests");
}
