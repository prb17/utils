#include <string>

#include "array.hh"
#include "validator.hh"
#include "logger.hh"

#include "heap.hh"

using namespace prb17::utils::structures;
static prb17::utils::logger logger{"heap_test"};

template<typename T>
static heap<T> build_min_heap(prb17::utils::parsers::json_parser jp) {
    heap<T> h{};
    auto input = jp.as_array<T>("input");
    for (size_t i=0; i<input.size(); i++) { h.push(input[i]); }
    return h;
}

// popping a min-heap yields the elements in ascending order
template<typename T>
bool testHeapPopOrder(prb17::utils::parsers::json_parser jp) {
    heap<T> h = build_min_heap<T>(jp);
    array<T> got{};
    while (!h.empty()) { got.add(h.pop()); }
    auto expected = jp.as_array<T>("expected");
    logger.debug("heap pop order: {}, expected: {}", got, expected);
    return got == expected;
}

// top() is the minimum without removing it
template<typename T>
bool testHeapTop(prb17::utils::parsers::json_parser jp) {
    heap<T> h = build_min_heap<T>(jp);
    T expected = jp.as_value<T>("expected_top");
    bool result = (h.top() == expected) && (h.size() == jp.as_array<T>("input").size());
    logger.debug("heap top: '{}', expected: '{}'", h.top(), expected);
    return result;
}

// size tracks the number of pushed elements
template<typename T>
bool testHeapSize(prb17::utils::parsers::json_parser jp) {
    heap<T> h = build_min_heap<T>(jp);
    int expected = jp.as_int("expected_size");
    return (int)h.size() == expected;
}

// a fresh heap is empty
template<typename T>
bool testHeapEmpty(prb17::utils::parsers::json_parser) {
    heap<T> h{};
    return h.empty() && h.size() == 0;
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;
    tests.add(prb17::utils::test{"testHeapPopOrder", &testHeapPopOrder<T>});
    tests.add(prb17::utils::test{"testHeapTop", &testHeapTop<T>});
    tests.add(prb17::utils::test{"testHeapSize", &testHeapSize<T>});
    tests.add(prb17::utils::test{"testHeapEmpty", &testHeapEmpty<T>});
    return tests;
}
template<typename T>
static prb17::utils::structures::array<prb17::utils::test> heap_tests = build_tests<T>();

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
    validator.add_tests(heap_tests<int>);

    logger.info("Starting validation tests of heap_tests");
    validator.validate();
    logger.info("Finished validation tests of heap_tests");
}
