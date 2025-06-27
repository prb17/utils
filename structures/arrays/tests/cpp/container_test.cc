#include <cassert>

#include "container.hh"
#include "validator.hh"
#include "logger.hh"

static prb17::utils::logger logger{"container_test"};

using namespace prb17::utils::structures;

template<typename T>
bool testContainerEquals(prb17::utils::parsers::json_parser jp) {
    container<T> c1 = jp.as_value<T>("container1");
    container<T> c2 = jp.as_value<T>("container2");

    bool expected = jp.as_bool("expected");
    bool result = c1 == c2;
    logger.debug("expected equals: '{}', result equals: '{}'", expected ? "true":"false", result ? "true":"false");
    return expected == result;
}

template<typename T>
bool testContainerNotEquals(prb17::utils::parsers::json_parser jp) {
    return !testContainerEquals<T>(jp);
}

template<typename T>
bool testContainerLessThan(prb17::utils::parsers::json_parser jp) {
    container<T> c1 = jp.as_value<T>("container1");
    container<T> c2 = jp.as_value<T>("container2");

    bool expected = jp.as_bool("expected");
    bool result = c1 < c2;
    logger.debug("expected equals: '{}', result equals: '{}'", expected ? "true":"false", result ? "true":"false");
    return expected == result;
}

template<typename T>
bool testContainerGreaterThanEqual(prb17::utils::parsers::json_parser jp) {
    return !testContainerLessThan<T>(jp);
}

template<typename T>
bool testContainerGreaterThan(prb17::utils::parsers::json_parser jp) {
    container<T> c1 = jp.as_value<T>("container1");
    container<T> c2 = jp.as_value<T>("container2");

    bool expected = jp.as_bool("expected");
    bool result = c1 > c2;
    logger.debug("expected equals: '{}', result equals: '{}'", expected ? "true":"false", result ? "true":"false");
    return expected == result;
}

template<typename T>
bool testContainerLessThanEqual(prb17::utils::parsers::json_parser jp) {
    return !testContainerGreaterThan<T>(jp);
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;

    tests.add(prb17::utils::test{"testContainerEquals", &testContainerEquals<T>});
    tests.add(prb17::utils::test{"testContainerNotEquals", &testContainerNotEquals<T>});
    tests.add(prb17::utils::test{"testContainerLessThan", &testContainerLessThan<T>});
    tests.add(prb17::utils::test{"testContainerGreaterThanEqual", &testContainerGreaterThanEqual<T>});
    tests.add(prb17::utils::test{"testContainerGreaterThan", &testContainerGreaterThan<T>});
    tests.add(prb17::utils::test{"testContainerLessThanEqual", &testContainerLessThanEqual<T>});

    return tests;
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> container_tests = build_tests<T>(); 

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
    
    validator.add_tests(container_tests<std::string>);
    validator.add_tests(container_tests<int>);
    validator.add_tests(container_tests<uint>);
    validator.add_tests(container_tests<char>);
    validator.add_tests(container_tests<bool>);
    validator.add_tests(container_tests<float>);
    validator.add_tests(container_tests<double>);

    logger.info("Starting validation tests of container_tests");
    validator.validate();
    logger.info("Finished validation tests of container_tests");
}
