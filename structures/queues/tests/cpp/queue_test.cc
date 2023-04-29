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
    logger.debug("Queue after all dequeues", queue);

    return queue.empty();   
}

//Map that relates the json file test config file to each test function defined in this file
template<typename T>
static std::map<std::string, std::function<bool(prb17::utils::parsers::json_parser)> > queue_tests = {
    {"testQueueEnqueue", &testQueueEnqueue<T>},
    {"testQueueDequeue", &testQueueDequeue<T>}
};


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
    
    validator.add_tests(&queue_tests<std::string>);
    validator.add_tests(&queue_tests<int>);
    validator.add_tests(&queue_tests<uint>);
    validator.add_tests(&queue_tests<char>);
    validator.add_tests(&queue_tests<bool>);
    validator.add_tests(&queue_tests<float>);
    validator.add_tests(&queue_tests<double>);

    logger.info("Starting validation tests of queue_tests");
    validator.validate();
    logger.info("Finished validation tests of queue_tests");
}
