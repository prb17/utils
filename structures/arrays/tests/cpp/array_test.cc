#include <iostream>
#include <cassert>
#include <map>

#include "array.hh"
#include "validator.hh"
#include "logger.hh"

static prb17::utils::logger logger{"array_test"};

// void testOperators() {
//    array<int> arr1{};
//     arr1.add(3);
//     arr1.add(6);
//     arr1.add(9);
//     arr1.add(12);
//     arr1.add(15);
//     std::cout << "arr1: ";
//     debugArray(arr1);

//     array<int> arr2{};
//     assert(arr2 != arr1);

//     arr2 = arr1;
//     std::cout << "arr2: ";
//     debugArray(arr2);
//     assert(arr2 == arr1);

//     assert(arr1[3] == arr2[3]);

//     arr1[3] = 4;
//     std::cout << "arr1: ";
//     debugArray(arr1);
//     assert(arr1[3] != arr2[3]);
//     assert(arr1 != arr2);

//     arr2[3] = arr1[3];
//     assert(arr1 == arr2);

//     array<int> arr3{arr2};
//     assert(arr2 == arr3);
    
//     array<int> arr4 = arr3;
//     assert(arr3 == arr4);
// }

// void testArrayClear() {
//    prb17::utils::structures::array<int> my_array{};
//     my_array.add(3);
//     my_array.add(6);
//     my_array.add(9);
//     my_array.add(12);
//     my_array.add(15);
//     assert(my_array.size() == 5);

//     my_array.clear();
//     assert(my_array.size() == 0);
//     assert(my_array.empty());

// }

template<typename T>
bool testArrayRemove(prb17::utils::parsers::json_parser jp) {
    auto arr = jp.as_array<T>("array");

    logger.debug("input array: {}", arr);

    auto rem_value = jp.as_value<T>("remove");
    logger.debug("removing value: '{}'", rem_value);

    auto rem_idx = arr.find(rem_value);
    logger.debug("removing value at index: '{}'", rem_idx);
    
    int expected = jp.as_int("expected");
    logger.debug("expected index: '{}'", expected);

    int result = arr.find(rem_value);
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
    int result = arr.find(ins_value);
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

    int result = arr.find(find_value);
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

//Map that relates the json file test config file to each test function defined in this file
template<typename T>
static std::map<std::string, std::function<bool(prb17::utils::parsers::json_parser)> > array_tests = {
    {"testDefaultConstructor", &testDefaultConstructor<T>},
    {"testCapacityOnlyConstructor", &testCapacityOnlyConstructor<T>},
    {"testArrayFind", &testArrayFind<T>},
    {"testArrayAdd", &testArrayAdd<T>},
    {"testArrayInsert", &testArrayInsert<T>},
    {"testArrayRemove", &testArrayRemove<T>}
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
    
    validator.add_tests(&array_tests<std::string>);
    validator.add_tests(&array_tests<int>);
    validator.add_tests(&array_tests<uint>);
    validator.add_tests(&array_tests<char>);
    validator.add_tests(&array_tests<bool>);
    validator.add_tests(&array_tests<float>);
    validator.add_tests(&array_tests<double>);

    logger.info("Starting validation tests of array_tests");
    validator.validate();
    logger.info("Finished validation tests of array_tests");
}
