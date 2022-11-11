#include <iostream>
#include <cassert>

#include "array.hh"

void debugArray(prb17::utils::structures::array<int> &arr) {
    std::cout << arr << std::endl;
}

void testOperators() {
   prb17::utils::structures::array<int> arr1{};
    arr1.add(3);
    arr1.add(6);
    arr1.add(9);
    arr1.add(12);
    arr1.add(15);
    std::cout << "arr1: ";
    debugArray(arr1);

    prb17::utils::structures::array<int> arr2{};
    assert(arr2 != arr1);

    arr2 = arr1;
    std::cout << "arr2: ";
    debugArray(arr2);
    assert(arr1 == arr2);

    assert(arr1[3] == arr2[3]);

    arr1[3] = 4;
    std::cout << "arr1: ";
    debugArray(arr1);
    assert(arr1[3] != arr2[3]);
    assert(arr1 != arr2);

    arr2[3] = arr1[3];
    assert(arr1 == arr2);

    prb17::utils::structures::array<int> arr3{arr2};
    assert(arr2 == arr3);
    
    prb17::utils::structures::array<int> arr4 = arr3;
    assert(arr3 == arr4);
    
    // pbrady::utils::structures::array<int> arr5 = {1, 2, 4}; //todo: implement creating array from <brace-enclosed initializer list>
    // assert(arr5 == {1, 2, 4});
}

void testArrayFind() {
   prb17::utils::structures::array<int> my_array{};
    my_array.add(3);
    my_array.add(6);
    my_array.add(9);
    my_array.add(12);
    my_array.add(15);

    assert(my_array.find(12) == 3);
    assert(my_array.find(20) == -1);

    int i = my_array.find(6);
    std::cout << my_array[i] << std::endl;

    debugArray(my_array);
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

int main() {
    
    //constructor tests
    testDefaultConstructor();
    testSizeOnlyConstructor();
    testFullArrayConstructor();

    //modifier tests
    testArrayAdd();
    testArrayInsert();
    testArrayRemove();
    testArrayClear();
    testArrayFind();

    //accessor tests
    //accessors are used to test the modifiers, making the
    //decision not to test them for now
    testOperators();
}
