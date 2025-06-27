#include <iostream>
#include <cassert>

#include "array.hh"
#include "validator.hh"
#include "logger.hh"
#include "map.hh"

static prb17::utils::logger logger{"array_test"};

template<typename T>
bool testArraySubscriptOperator(prb17::utils::parsers::json_parser jp) {
    auto arr1 = jp.as_array<T>("array1");
    logger.debug("input array1: {}", arr1);

    auto arr2 = jp.as_array<T>("array2");
    logger.debug("input array2: {}", arr2);

    int index1 = jp.as_int("index1");
    int index2 = jp.as_int("index2");
    logger.debug("evaluating arr1 at index: '{}', against arr2 at index: '{}'", index1, index2);
    
    bool expected = jp.as_bool("expected");
    bool result = arr1[index1] == arr2[index2];
    logger.debug("expected equals: '{}', result equals: '{}'", expected ? "true":"false", result ? "true":"false");
    return expected == result;
}

template<typename T>
bool testArrayEqualsOperator(prb17::utils::parsers::json_parser jp) {
    auto arr1 = jp.as_array<T>("array1");
    logger.debug("input array1: {}", arr1);

    auto arr2 = jp.as_array<T>("array2");
    logger.debug("input array2: {}", arr2);
    
    bool expected = jp.as_bool("expected");
    bool result = arr1 == arr2;
    logger.debug("expected equals: '{}', result equals: '{}'", expected, result);
    return expected == result;
}

template<typename T>
bool testArrayNotEqualsOperator(prb17::utils::parsers::json_parser jp) {
    return !testArrayEqualsOperator<T>(jp);
}

template<typename T>
bool testArrayClear(prb17::utils::parsers::json_parser jp) {
    auto arr = jp.as_array<T>("array");

    logger.debug("input array: {}", arr);
    
    int expected = jp.as_int("expected");
    logger.debug("expected size: '{}'", expected);

    arr.clear();
    int result = arr.size();
    logger.debug("array after: {}", arr);
    return expected == result;
}

template<typename T>
bool testArrayRemove(prb17::utils::parsers::json_parser jp) {
    auto arr = jp.as_array<T>("array");

    logger.debug("input array: {}", arr);

    auto rem_value = jp.as_value<T>("remove");
    //auto rem_idx = arr.find(rem_value);
    auto rem_idx = -1;
    logger.debug("removing value: '{}' at idx: '{}'", rem_value, rem_idx);
    
    int expected = jp.as_int("expected");
    logger.debug("expected index: '{}'", expected);

    arr.remove(rem_idx);
    //int result = arr.find(rem_value);
    int result = -1;
    logger.debug("result index was: '{}'", result);

    logger.debug("array after: {}", arr);
    return expected == result;
}

template<typename T>
bool testArrayInsert(prb17::utils::parsers::json_parser jp) {
    auto arr = jp.as_array<T>("array");

    logger.debug("input array: {}", arr);

    auto ins_value = jp.as_value<T>("insert");
    logger.debug("inserting value: '{}'", ins_value);

    auto ins_idx = jp.as_int("idx");
    logger.debug("inserting value at index: '{}'", ins_idx);
    
    int expected = jp.as_int("expected");
    logger.debug("expected index: '{}'", expected);

    arr.insert(ins_idx, ins_value);
    //int result = arr.find(ins_value);
    int result = -1;
    logger.debug("result index was: '{}'", result);

    return expected == result;
}

template<typename T>
bool testArrayAdd(prb17::utils::parsers::json_parser jp)  {
    auto arr = jp.as_array<T>("array");
    logger.debug("input array: {}, size: '{}', cap: '{}'", arr, arr.size(), arr.capacity());

    auto add_value = jp.as_value<T>("add");
    logger.debug("adding value: '{}'", add_value);

    int expected = jp.as_int("expected");
    logger.debug("expected index: '{}'", expected);

    arr.add(add_value);
    logger.debug("array after add: size: '{}', cap: '{}'", arr.size(), arr.capacity());
    logger.debug("array after add: '{}'", arr);
    T last_val = arr[arr.size()-1];
    return last_val == add_value;
}

template<typename T>
bool testArrayFind(prb17::utils::parsers::json_parser jp) {
    auto arr = jp.as_array<T>("array");

    logger.debug("input array: {}", arr);

    auto find_value = jp.as_value<T>("find");
    logger.debug("finding value: '{}'", find_value);
    
    int expected = jp.as_int("expected");
    logger.debug("expected index: '{}'", expected);

    //int result = arr.find(find_value);
    int result = -1;
    logger.debug("result index was: '{}'", result);

    return expected == result;   
}
template<typename T>
bool testCapacityOnlyConstructor(prb17::utils::parsers::json_parser jp) {
    size_t cap = jp.as_value<int>("capacity");
    prb17::utils::structures::array<T> my_array{cap};
    bool retval = true;
    retval &= my_array.size() == 0;
    retval &= my_array.capacity() == cap;
    retval &= my_array.empty();

    return retval;
}

template<typename T>
bool testDefaultConstructor(prb17::utils::parsers::json_parser jp) {
    auto my_array = jp.as_array<T>("array");
    bool retval= true;
    retval &= my_array.size() == 0;
    retval &= my_array.capacity() == 0;

    return retval;
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;

    tests.add(prb17::utils::test("testDefaultConstructor", &testDefaultConstructor<T>)); 
    tests.add(prb17::utils::test("testCapacityOnlyConstructor", &testCapacityOnlyConstructor<T>)); 
    tests.add(prb17::utils::test("testArrayFind", &testArrayFind<T>)); 
    tests.add(prb17::utils::test("testArrayAdd", &testArrayAdd<T>)); 
    tests.add(prb17::utils::test("testArrayInsert", &testArrayInsert<T>)); 
    tests.add(prb17::utils::test("testArrayRemove", &testArrayRemove<T>)); 
    tests.add(prb17::utils::test("testArrayClear", &testArrayClear<T>)); 
    tests.add(prb17::utils::test("testArrayEqualsOperator", &testArrayEqualsOperator<T>)); 
    tests.add(prb17::utils::test("testArrayNotEqualsOperator", &testArrayNotEqualsOperator<T>)); 
    tests.add(prb17::utils::test("testArraySubscriptOperator", &testArraySubscriptOperator<T>)); 

    return tests;
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> array_tests = build_tests<T>();

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

    validator.add_tests(array_tests<std::string>);
    validator.add_tests(array_tests<int>);
    validator.add_tests(array_tests<uint>);
    validator.add_tests(array_tests<char>);
    validator.add_tests(array_tests<bool>);
    validator.add_tests(array_tests<float>);
    validator.add_tests(array_tests<double>);

    logger.info("Starting validation tests of array_tests");
    validator.validate();
    logger.info("Finished validation tests of array_tests");
}
