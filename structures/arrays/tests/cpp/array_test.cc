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
    
    // prb17::utils::structures::array<int> arr5 = {1, 2, 4}; //todo: implement creating array from <brace-enclosed initializer list>
    // assert(arr5 == {1, 2, 4});
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

void testFullArrayConstructor() {
    size_t size = 3;
    int data[] = {1, 2, 3};
   prb17::utils::structures::array<int> my_array(data, size);
    assert(my_array.size() == size);
    assert(my_array.capacity() == 2*size);
    
    assert(my_array.get(0) == data[0]);
    assert(my_array.get(1) == data[1]);
    assert(my_array.get(2) == data[2]);
}

void testSizeOnlyConstructor() {
    size_t size = 5;
    prb17::utils::structures::array<int> my_array{size};
    assert(my_array.size() == size);
    assert(my_array.capacity() == 2*size);

    bool exception_happened = false;
    prb17::utils::structures::container<int> val;
    try {
        val = my_array.get(1);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        exception_happened = true;
    }
    assert(!exception_happened);
    assert(val == 0);
}

void testDefaultConstructor() {
   prb17::utils::structures::array<int> my_array{};
    assert(my_array.size() == 0);
    assert(my_array.capacity() == 0);

    bool exception_happened = false;
    try {
        my_array.get(1);
    } catch (const prb17::utils::exception& e) {
        std::cout << e.what() << std::endl;
        exception_happened = true;
    } catch(...) {}
    assert(exception_happened);
}

// void testStringArray() {
//    prb17::utils::structures::array<std::string> arr{};
//     arr.add("one");
//     arr.add("two");
//     arr.add("two");
//     arr.add("two");
//     arr.add("two");
//     arr.add("dafdafdafdas");
//     arr.add("two");
//     arr.add("two");
//     arr.add("two");
//     arr.add("dasfdsafdsafdsa");
//     arr.add("two");
//     arr.add("two");
//     arr.add("twdewfefeo");
//     arr.add("two");

// }

//TODO: This is the target I want to reach, but I'm not sure how to get here or if it is even possible
template<typename T>
bool testGenericArrayFind(parsers::json_parser jp) {
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

// TODO: Can this be better than a wrapper? Can the two be combined into 1 functions?
// It's still way better like this, but maybe I can somehow get the validate class to 
//  work with templatized functions itself... But then if a test wants to work with a
//  particular datatype, how would it handle that? 
bool testArrayFindWrapper(parsers::json_parser jp) {
    std::string datatype = jp.as_string("datatype");
    if (datatype == "string") {
        return testGenericArrayFind<std::string>(jp);
    } else if (datatype == "int") {
        return testGenericArrayFind<int>(jp);
    } else if (datatype == "uint") {
        return testGenericArrayFind<uint>(jp);
    } else if (datatype == "char") {
        return testGenericArrayFind<char>(jp);
    }  else if (datatype == "bool") {
        return testGenericArrayFind<bool>(jp);
    } else if (datatype == "float") {
        return testGenericArrayFind<float>(jp);
    } else if (datatype == "double") {
        return testGenericArrayFind<double>(jp);
    } else {
        throw exception("Unsupported datatype for testArrayFind");
    }    
}

//Map that relates the json file test config file to each test function defined in this file
static std::map<std::string, std::function<bool(prb17::utils::parsers::json_parser)> > test_map = {
    {"testArrayFind", &testArrayFindWrapper}
};

#define NUM_ARGS 2
int main(int argc, char** argv) {
    if (argc != NUM_ARGS) {
        throw prb17::utils::exception("This test requires a config file to be provided");
    }
    std::string test_file(&argv[1][0]);

    prb17::utils::validator validator{test_file, &test_map};
    validator.validate();

    // testStringArray();
}
