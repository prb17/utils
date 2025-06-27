#include <iostream>
#include <cassert>

#include "stack.hh"
#include "validator.hh"
#include "logger.hh"

static prb17::utils::logger logger{"stack_test"};

template<typename T>
bool testStackPush(prb17::utils::parsers::json_parser jp) {
    prb17::utils::structures::stack<T> stack{};
    auto vals = jp.as_array<T>("values");
    logger.debug("adding vals: {} to empty stack", vals);
    for(int i=0; i<vals.size(); i++) {
        stack.push(vals[i]);
    }
    logger.debug("Final stack: {}", stack);

    return stack == vals;   
}

template<typename T>
bool testStackPop(prb17::utils::parsers::json_parser jp) {
    prb17::utils::structures::stack<T> stack = jp.as_array<T>("values");
    logger.debug("Original stack: {}", stack);
    while (!stack.empty()) {
        T back = stack.pop();
        logger.debug("Popped value: '{}' off stack", back);
    }
    logger.debug("Stack after all pops", stack);

    return stack.empty();   
}

//Map that relates the json file test config file to each test function defined in this file
template<typename T>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;

    tests.add(prb17::utils::test{"testStackPush", &testStackPush<T>});
    tests.add(prb17::utils::test{"testStackPop", &testStackPop<T>});

    return tests;
};

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> stack_tests = build_tests<T>(); 

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
    
    validator.add_tests(stack_tests<std::string>);
    validator.add_tests(stack_tests<int>);
    validator.add_tests(stack_tests<uint>);
    validator.add_tests(stack_tests<char>);
    validator.add_tests(stack_tests<bool>);
    validator.add_tests(stack_tests<float>);
    validator.add_tests(stack_tests<double>);

    logger.info("Starting validation tests of stack_tests");
    validator.validate();
    logger.info("Finished validation tests of stack_tests");
}
