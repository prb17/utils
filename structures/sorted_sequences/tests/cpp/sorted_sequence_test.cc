#include <string>

#include "array.hh"
#include "validator.hh"
#include "logger.hh"

#include "sorted_sequence.hh"

using namespace prb17::utils::structures;
static prb17::utils::logger logger{"sorted_sequence_test"};

template<typename T>
static sorted_sequence<T> build_sorted(prb17::utils::parsers::json_parser jp) {
    sorted_sequence<T> s{};
    auto input = jp.as_array<T>("input");
    for (size_t i=0; i<input.size(); i++) { s.insert(input[i]); }
    return s;
}

template<typename T>
static array<T> to_array(const sorted_sequence<T>& s) {
    array<T> out{};
    for (size_t i=0; i<s.size(); i++) { out.add(s[i]); }
    return out;
}

// inserting in any order yields sorted contents
template<typename T>
bool testSortedOrder(prb17::utils::parsers::json_parser jp) {
    sorted_sequence<T> s = build_sorted<T>(jp);
    array<T> got = to_array(s);
    auto expected = jp.as_array<T>("expected");
    logger.debug("sorted order: {}, expected: {}", got, expected);
    return got == expected;
}

// contains reflects membership
template<typename T>
bool testSortedContains(prb17::utils::parsers::json_parser jp) {
    sorted_sequence<T> s = build_sorted<T>(jp);
    T present = jp.as_value<T>("present");
    T absent = jp.as_value<T>("absent");
    return s.contains(present) && !s.contains(absent);
}

// find returns the binary-search index (or -1)
template<typename T>
bool testSortedFind(prb17::utils::parsers::json_parser jp) {
    sorted_sequence<T> s = build_sorted<T>(jp);
    T value = jp.as_value<T>("value");
    int expected = jp.as_int("expected_index");
    logger.debug("find '{}' -> '{}', expected '{}'", value, s.find(value), expected);
    return s.find(value) == expected;
}

// remove deletes one occurrence and keeps order; removing an absent value fails
template<typename T>
bool testSortedRemove(prb17::utils::parsers::json_parser jp) {
    sorted_sequence<T> s = build_sorted<T>(jp);
    T value = jp.as_value<T>("remove");
    T absent = jp.as_value<T>("absent");

    bool removed = s.remove(value);
    bool absent_removed = s.remove(absent);
    array<T> got = to_array(s);
    auto expected = jp.as_array<T>("expected");
    logger.debug("after remove: {}, expected: {}", got, expected);
    return removed && !absent_removed && got == expected;
}

// min and max are the ends of the sorted sequence
template<typename T>
bool testSortedMinMax(prb17::utils::parsers::json_parser jp) {
    sorted_sequence<T> s = build_sorted<T>(jp);
    return (s.min() == jp.as_value<T>("expected_min"))
        && (s.max() == jp.as_value<T>("expected_max"));
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;
    tests.add(prb17::utils::test{"testSortedOrder", &testSortedOrder<T>});
    tests.add(prb17::utils::test{"testSortedContains", &testSortedContains<T>});
    tests.add(prb17::utils::test{"testSortedFind", &testSortedFind<T>});
    tests.add(prb17::utils::test{"testSortedRemove", &testSortedRemove<T>});
    tests.add(prb17::utils::test{"testSortedMinMax", &testSortedMinMax<T>});
    return tests;
}
template<typename T>
static prb17::utils::structures::array<prb17::utils::test> sorted_tests = build_tests<T>();

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
    validator.add_tests(sorted_tests<int>);

    logger.info("Starting validation tests of sorted_sequence_tests");
    validator.validate();
    logger.info("Finished validation tests of sorted_sequence_tests");
}
