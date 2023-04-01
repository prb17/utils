#include <iostream>
#include <cassert>
#include <map>

#include "array.hh"
#include "validator.hh"

using namespace prb17::utils;
using namespace prb17::utils::structures;

void debugArray(array<int> &arr) {
    std::cout << arr << std::endl;
}

void testOperators() {
   array<int> arr1{};
    arr1.add(3);
    arr1.add(6);
    arr1.add(9);
    arr1.add(12);
    arr1.add(15);
    std::cout << "arr1: ";
    debugArray(arr1);

    array<int> arr2{};
    assert(arr2 != arr1);

    arr2 = arr1;
    std::cout << "arr2: ";
    debugArray(arr2);
    assert(arr2 == arr1);

    assert(arr1[3] == arr2[3]);

    arr1[3] = 4;
    std::cout << "arr1: ";
    debugArray(arr1);
    assert(arr1[3] != arr2[3]);
    assert(arr1 != arr2);

    arr2[3] = arr1[3];
    assert(arr1 == arr2);

    array<int> arr3{arr2};
    assert(arr2 == arr3);
    
    array<int> arr4 = arr3;
    assert(arr3 == arr4);
}

void testArrayClear() {
   prb17::utils::structures::array<int> my_array{};
    my_array.add(3);
    my_array.add(6);
    my_array.add(9);
    my_array.add(12);
    my_array.add(15);
    assert(my_array.size() == 5);

    my_array.clear();
    assert(my_array.size() == 0);
    assert(my_array.empty());

}

void testArrayRemove() {
   prb17::utils::structures::array<int> my_array{};
    my_array.add(3);
    my_array.add(6);
    my_array.add(9);
    my_array.add(12);
    my_array.add(15);
    assert(my_array.size() == 5);

    my_array.remove(2);
    assert(my_array.get(2) == 12);
    assert(my_array.size() == 4);
}

void testArrayInsert() {
   prb17::utils::structures::array<int> my_array{};
    my_array.add(3);
    my_array.add(6);
    my_array.add(9);
    my_array.add(12);
    my_array.add(15);
    assert(my_array.size() == 5);

    my_array.insert(2, 200);
    assert(my_array.get(2) == 200);
    assert(my_array.size() == 6);
}

void testArrayAdd() {
   prb17::utils::structures::array<int> my_array{};
    assert(my_array.size() == 0);
    assert(my_array.capacity() == 0);

    my_array.add(3);
    assert(my_array.size() == 1);
    assert(my_array.capacity() == 2);
    
    my_array.add(6);
    assert(my_array.size() == 2);
    assert(my_array.capacity() == 2);

    my_array.add(12);
    assert(my_array.size() == 3);
    assert(my_array.capacity() == 6);
}

template<typename T>
bool testArrayFind(parsers::json_parser jp) {
    auto arr = jp.as_array<T>("array");

    std::cout << "input array : " << arr << std::endl;
    int expected = jp.as_int("expected");
    std::cout << "expected index: " << expected << std::endl;

    auto find_value = jp.as_value<T>("find");
    std::cout << "finding value: " << find_value << std::endl;

    int result = arr.find(find_value);
    std::cout << "result index was: " << result << std::endl;

    return expected == result;   
}

template<typename T>
bool testCapacityOnlyConstructor(parsers::json_parser jp) {
    size_t cap = jp.as_value<int>("capacity");
    prb17::utils::structures::array<T> my_array{cap};
    bool retval = true;
    retval &= my_array.size() == 0;
    retval &= my_array.capacity() == cap;
    retval &= my_array.empty();

    return retval;
}

template<typename T>
bool testDefaultConstructor(parsers::json_parser jp) {
    auto my_array = jp.as_array<T>("array");
    bool retval= true;
    retval &= my_array.size() == 0;
    retval &= my_array.capacity() == 0;

    return retval;
}

//Map that relates the json file test config file to each test function defined in this file
template<typename T>
std::map<std::string, std::function<bool(prb17::utils::parsers::json_parser)> > test_map = {
    {"testDefaultConstructor", &testDefaultConstructor<T>},
    {"testCapacityOnlyConstructor", &testCapacityOnlyConstructor<T>},
    {"testArrayFind", &testArrayFind<T>}
};

#define MIN_NUM_ARGS 2
int main(int argc, char** argv) {
    if (argc < MIN_NUM_ARGS) {
        throw prb17::utils::exception("This test requires a config file to be provided");
    }
    prb17::utils::structures::array<std::string> test_files{};
    for (int i=1; i<argc; i++) {
        std::cout << "adding test file to validator: " << &argv[i][0] << std::endl;
        test_files.add(&argv[i][0]);
    }

    prb17::utils::validator validator{test_files};
    
    validator.add_tests(&test_map<std::string>);
    validator.add_tests(&test_map<int>);
    validator.add_tests(&test_map<uint>);
    validator.add_tests(&test_map<char>);
    validator.add_tests(&test_map<bool>);
    validator.add_tests(&test_map<float>);
    validator.add_tests(&test_map<double>);

    validator.validate();
}
