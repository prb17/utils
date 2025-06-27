#include "validator.hh"
#include "logger.hh"
#include "set.hh"

#include <iostream>
#include <memory>
#include <set>

using namespace prb17::utils::structures;
static prb17::utils::logger logger{"set_test"};

template<typename T>
bool basic_set_print(prb17::utils::parsers::json_parser jp) {
    logger.info("Building set");
    set s = set<T>{};
    return true;
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;
    tests.add(prb17::utils::test{"basicSetPrint", &basic_set_print<T>});
    return tests;
};
template<typename T>
static prb17::utils::structures::array<prb17::utils::test> set_tests = build_tests<T>();

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
    
    validator.add_tests(set_tests<std::string>);
    validator.add_tests(set_tests<int>);
    validator.add_tests(set_tests<uint>);
    validator.add_tests(set_tests<char>);
    validator.add_tests(set_tests<bool>);
    validator.add_tests(set_tests<float>);
    validator.add_tests(set_tests<double>);

    logger.info("Starting validation tests of set_test");
    validator.validate();
    logger.info("Finished validation tests of set_test");
}
