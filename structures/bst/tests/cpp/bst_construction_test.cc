#include <string>

#include "array.hh"
#include "validator.hh"
#include "logger.hh"

#include "bst.hh"

using namespace prb17::utils::structures;
static prb17::utils::logger logger{"bst_construction_test"};

// in-order traversal is a read-only operation, so it lives in the test rather
// than on the bst structure; on a valid BST it yields the values in sorted order
static void in_order(bst<int>* node, array<int>& out) {
    if (node == nullptr) { return; }
    in_order(node->left, out);
    out.add(node->value);
    in_order(node->right, out);
}

static bst<int>* build(prb17::utils::parsers::json_parser jp) {
    auto values = jp.as_array<int>("values");
    bst<int>* root = new bst<int>(values[0]);
    for (size_t i=1; i<values.size(); i++) {
        root->insert(values[i]);
    }
    return root;
}

// contains finds present values and rejects absent ones
bool testContains(prb17::utils::parsers::json_parser jp) {
    bst<int>* root = build(jp);
    bool result = root->contains(jp.as_int("present"))
               && !root->contains(jp.as_int("absent"));
    delete root;
    return result;
}

// inserting in any order produces a valid BST: its in-order traversal is sorted
bool testInorderSorted(prb17::utils::parsers::json_parser jp) {
    bst<int>* root = build(jp);
    array<int> got{};
    in_order(root, got);
    auto expected = jp.as_array<int>("expected");
    logger.debug("in-order: {}, expected: {}", got, expected);
    delete root;
    return got == expected;
}

// remove covers leaf / one-child / two-child / root / absent cases (per config);
// the tree stays a valid BST afterward and the value's presence matches expectation
bool testRemove(prb17::utils::parsers::json_parser jp) {
    bst<int>* root = build(jp);
    int value = jp.as_int("remove");

    root = root->remove(value);

    array<int> got{};
    in_order(root, got);
    auto expected = jp.as_array<int>("expected");
    bool contains_after = (root != nullptr) && root->contains(value);
    bool expected_contains = jp.as_bool("expected_contains");

    logger.debug("after remove {}: {}, expected: {}", value, got, expected);
    delete root;
    return got == expected && contains_after == expected_contains;
}

// equal values go to the right subtree, so duplicates are kept and in-order
// traversal remains sorted (stable)
bool testDuplicatesGoRight(prb17::utils::parsers::json_parser jp) {
    bst<int>* root = build(jp);
    array<int> got{};
    in_order(root, got);
    auto expected = jp.as_array<int>("expected");
    logger.debug("duplicates in-order: {}, expected: {}", got, expected);
    delete root;
    return got == expected;
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

    validator.add_test("testContains", &testContains, "");
    validator.add_test("testInorderSorted", &testInorderSorted, "");
    validator.add_test("testRemove", &testRemove, "");
    validator.add_test("testDuplicatesGoRight", &testDuplicatesGoRight, "");

    logger.info("Starting validation tests of bst_construction_tests");
    validator.validate();
    logger.info("Finished validation tests of bst_construction_tests");
}
