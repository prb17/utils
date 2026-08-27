#include <string>

#include "array.hh"
#include "validator.hh"
#include "logger.hh"

#include "ring_buffer.hh"

using namespace prb17::utils::structures;
static prb17::utils::logger logger{"ring_buffer_test"};

// pushing then popping preserves insertion order (FIFO)
template<typename T>
bool testRingFifo(prb17::utils::parsers::json_parser jp) {
    ring_buffer<T> rb{(size_t)jp.as_int("capacity")};
    auto input = jp.as_array<T>("input");
    for (size_t i=0; i<input.size(); i++) { rb.push(input[i]); }

    array<T> got{};
    while (!rb.empty()) { got.add(rb.pop()); }
    logger.debug("ring fifo: {}, expected: {}", got, input);
    return got == input;
}

// once full, push is rejected and size stays at capacity
template<typename T>
bool testRingFull(prb17::utils::parsers::json_parser jp) {
    size_t cap = (size_t)jp.as_int("capacity");
    ring_buffer<T> rb{cap};
    for (size_t i=0; i<cap; i++) { rb.push((T)i); }

    bool result = rb.full();
    result &= (rb.push(T{99}) == false); // rejected when full
    result &= (rb.size() == cap);
    return result;
}

// indices wrap around the end of the backing store: fill, drain some, refill
template<typename T>
bool testRingWraparound(prb17::utils::parsers::json_parser jp) {
    ring_buffer<T> rb{(size_t)jp.as_int("capacity")};

    auto first = jp.as_array<T>("first");
    for (size_t i=0; i<first.size(); i++) { rb.push(first[i]); }

    int pops = jp.as_int("pops");
    for (int i=0; i<pops; i++) { rb.pop(); }

    auto second = jp.as_array<T>("second");
    for (size_t i=0; i<second.size(); i++) { rb.push(second[i]); }

    array<T> got{};
    while (!rb.empty()) { got.add(rb.pop()); }
    auto expected = jp.as_array<T>("expected");
    logger.debug("ring wraparound: {}, expected: {}", got, expected);
    return got == expected;
}

// force_push overwrites the oldest element when full
template<typename T>
bool testRingForcePush(prb17::utils::parsers::json_parser jp) {
    ring_buffer<T> rb{(size_t)jp.as_int("capacity")};
    auto input = jp.as_array<T>("input");
    for (size_t i=0; i<input.size(); i++) { rb.force_push(input[i]); }

    array<T> got{};
    while (!rb.empty()) { got.add(rb.pop()); }
    auto expected = jp.as_array<T>("expected");
    logger.debug("ring force_push: {}, expected: {}", got, expected);
    return got == expected;
}

// a fresh buffer is empty and not full
template<typename T>
bool testRingEmpty(prb17::utils::parsers::json_parser jp) {
    ring_buffer<T> rb{(size_t)jp.as_int("capacity")};
    return rb.empty() && !rb.full() && rb.size() == 0;
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;
    tests.add(prb17::utils::test{"testRingFifo", &testRingFifo<T>});
    tests.add(prb17::utils::test{"testRingFull", &testRingFull<T>});
    tests.add(prb17::utils::test{"testRingWraparound", &testRingWraparound<T>});
    tests.add(prb17::utils::test{"testRingForcePush", &testRingForcePush<T>});
    tests.add(prb17::utils::test{"testRingEmpty", &testRingEmpty<T>});
    return tests;
}
template<typename T>
static prb17::utils::structures::array<prb17::utils::test> ring_tests = build_tests<T>();

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
    validator.add_tests(ring_tests<int>);

    logger.info("Starting validation tests of ring_buffer_tests");
    validator.validate();
    logger.info("Finished validation tests of ring_buffer_tests");
}
