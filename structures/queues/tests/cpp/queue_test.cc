#include <iostream>
#include <cassert>

#include "queue.hh"
#include "validator.hh"
#include "logger.hh"

static prb17::utils::logger logger{"queue_test"};


template<typename T>
bool testQueueEnqueue(prb17::utils::parsers::json_parser jp) {
    prb17::utils::structures::queue<T> queue{};
    auto vals = jp.as_array<T>("values");
    logger.debug("adding vals: {}, to empty queue", vals);
    for(int i=0; i<vals.size(); i++) {
        queue.enqueue(vals[i]);
    }
    logger.debug("Final queue: {}", queue);

    return queue == vals;   
}

template<typename T>
bool testQueueDequeue(prb17::utils::parsers::json_parser jp) {
    prb17::utils::structures::queue<T> queue = jp.as_array<T>("values");
    logger.debug("Original queue: {}", queue);
    while (!queue.empty()) {
        T front = queue.dequeue();
        logger.debug("Dequeued value: '{}' from front of queue", front);
        logger.debug("Queue now looks like: {}", queue);
    }
    logger.debug("Queue after all dequeues: {}", queue);

    return queue.empty();   
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;

    tests.add(prb17::utils::test{"testQueueEnqueue", &testQueueEnqueue<T>});
    tests.add(prb17::utils::test{"testQueueDequeue", &testQueueDequeue<T>});

    return tests;
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> queue_tests = build_tests<T>();

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
    
    validator.add_tests(queue_tests<std::string>);
    validator.add_tests(queue_tests<int>);
    validator.add_tests(queue_tests<uint>);
    validator.add_tests(queue_tests<char>);
    validator.add_tests(queue_tests<bool>);
    validator.add_tests(queue_tests<float>);
    validator.add_tests(queue_tests<double>);

    logger.info("Starting validation tests of queue_tests");
    validator.validate();
    logger.info("Finished validation tests of queue_tests");
}
