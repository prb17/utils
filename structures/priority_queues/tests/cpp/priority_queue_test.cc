#include <string>
#include <functional>

#include "array.hh"
#include "validator.hh"
#include "logger.hh"

#include "priority_queue.hh"

using namespace prb17::utils::structures;
static prb17::utils::logger logger{"priority_queue_test"};

// a min priority queue (default) dequeues smallest-first
template<typename T>
bool testPQMinOrder(prb17::utils::parsers::json_parser jp) {
    priority_queue<T> pq{};
    auto input = jp.as_array<T>("input");
    for (size_t i=0; i<input.size(); i++) { pq.enqueue(input[i]); }

    array<T> got{};
    while (!pq.empty()) { got.add(pq.dequeue()); }
    auto expected = jp.as_array<T>("expected");
    logger.debug("min pq order: {}, expected: {}", got, expected);
    return got == expected;
}

// a max priority queue (std::greater) dequeues largest-first
template<typename T>
bool testPQMaxOrder(prb17::utils::parsers::json_parser jp) {
    priority_queue<T, std::greater<T>> pq{};
    auto input = jp.as_array<T>("input");
    for (size_t i=0; i<input.size(); i++) { pq.enqueue(input[i]); }

    array<T> got{};
    while (!pq.empty()) { got.add(pq.dequeue()); }
    auto expected = jp.as_array<T>("expected_desc");
    logger.debug("max pq order: {}, expected: {}", got, expected);
    return got == expected;
}

// peek returns the highest-priority element without removing it
template<typename T>
bool testPQPeekSize(prb17::utils::parsers::json_parser jp) {
    priority_queue<T> pq{};
    auto input = jp.as_array<T>("input");
    for (size_t i=0; i<input.size(); i++) { pq.enqueue(input[i]); }

    T expected_peek = jp.as_value<T>("expected_peek");
    return (pq.peek() == expected_peek) && ((int)pq.size() == jp.as_int("expected_size"));
}

// a fresh priority queue is empty
template<typename T>
bool testPQEmpty(prb17::utils::parsers::json_parser) {
    priority_queue<T> pq{};
    return pq.empty() && pq.size() == 0;
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;
    tests.add(prb17::utils::test{"testPQMinOrder", &testPQMinOrder<T>});
    tests.add(prb17::utils::test{"testPQMaxOrder", &testPQMaxOrder<T>});
    tests.add(prb17::utils::test{"testPQPeekSize", &testPQPeekSize<T>});
    tests.add(prb17::utils::test{"testPQEmpty", &testPQEmpty<T>});
    return tests;
}
template<typename T>
static prb17::utils::structures::array<prb17::utils::test> pq_tests = build_tests<T>();

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
    validator.add_tests(pq_tests<int>);

    logger.info("Starting validation tests of priority_queue_tests");
    validator.validate();
    logger.info("Finished validation tests of priority_queue_tests");
}
