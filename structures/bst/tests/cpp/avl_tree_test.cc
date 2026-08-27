#include <string>

#include "array.hh"
#include "validator.hh"
#include "logger.hh"

#include "avl_tree.hh"

using namespace prb17::utils::structures;
static prb17::utils::logger logger{"avl_tree_test"};

template<typename T>
static void insert_input(avl_tree<T>& t, prb17::utils::parsers::json_parser jp) {
    auto input = jp.as_array<T>("input");
    for (size_t i=0; i<input.size(); i++) { t.insert(input[i]); }
}

// in-order traversal of a BST is sorted, regardless of insertion order
template<typename T>
bool testAvlInorderSorted(prb17::utils::parsers::json_parser jp) {
    avl_tree<T> t{};
    insert_input(t, jp);
    array<T> got = t.in_order();
    auto expected = jp.as_array<T>("expected");
    logger.debug("in-order: {}, expected: {}", got, expected);
    return got == expected;
}

// contains reflects membership
template<typename T>
bool testAvlContains(prb17::utils::parsers::json_parser jp) {
    avl_tree<T> t{};
    insert_input(t, jp);
    return t.contains(jp.as_value<T>("present")) && !t.contains(jp.as_value<T>("absent"));
}

// inserting sorted input (worst case for a naive BST) stays balanced: an AVL
// tree of n nodes has height O(log n), not O(n)
template<typename T>
bool testAvlBalancedHeight(prb17::utils::parsers::json_parser jp) {
    avl_tree<T> t{};
    insert_input(t, jp);
    int expected = jp.as_int("expected_height");
    logger.debug("height: '{}', expected: '{}'", t.height(), expected);
    return t.height() == expected && (int)t.size() == (int)jp.as_array<T>("input").size();
}

// remove deletes present values (keeping the tree a sorted, valid BST) and
// reports false for absent values
template<typename T>
bool testAvlRemove(prb17::utils::parsers::json_parser jp) {
    avl_tree<T> t{};
    insert_input(t, jp);

    auto to_remove = jp.as_array<T>("remove");
    bool all_removed = true;
    for (size_t i=0; i<to_remove.size(); i++) { all_removed &= t.remove(to_remove[i]); }
    bool absent_removed = t.remove(jp.as_value<T>("absent"));

    array<T> got = t.in_order();
    auto expected = jp.as_array<T>("expected");
    logger.debug("after remove: {}, expected: {}", got, expected);
    return all_removed && !absent_removed && got == expected
        && (int)t.size() == jp.as_int("expected_size");
}

// min and max walk to the leftmost/rightmost nodes
template<typename T>
bool testAvlMinMax(prb17::utils::parsers::json_parser jp) {
    avl_tree<T> t{};
    insert_input(t, jp);
    return (t.min() == jp.as_value<T>("expected_min"))
        && (t.max() == jp.as_value<T>("expected_max"));
}

// duplicate inserts are rejected (set semantics)
template<typename T>
bool testAvlDuplicateRejected(prb17::utils::parsers::json_parser) {
    avl_tree<T> t{};
    bool result = t.insert(T{5});
    result &= (t.insert(T{5}) == false);
    result &= (t.size() == 1);
    return result;
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;
    tests.add(prb17::utils::test{"testAvlInorderSorted", &testAvlInorderSorted<T>});
    tests.add(prb17::utils::test{"testAvlContains", &testAvlContains<T>});
    tests.add(prb17::utils::test{"testAvlBalancedHeight", &testAvlBalancedHeight<T>});
    tests.add(prb17::utils::test{"testAvlRemove", &testAvlRemove<T>});
    tests.add(prb17::utils::test{"testAvlMinMax", &testAvlMinMax<T>});
    tests.add(prb17::utils::test{"testAvlDuplicateRejected", &testAvlDuplicateRejected<T>});
    return tests;
}
template<typename T>
static prb17::utils::structures::array<prb17::utils::test> avl_tests = build_tests<T>();

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
    validator.add_tests(avl_tests<int>);

    logger.info("Starting validation tests of avl_tree_tests");
    validator.validate();
    logger.info("Finished validation tests of avl_tree_tests");
}
