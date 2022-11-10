#include <iostream>
#include <cassert>

#include "array.hh"

void debugArray(pbrady::utils::structures::array<int> &arr) {
    std::cout << arr << std::endl;
}

void testArrayFind() {
   pbrady::utils::structures::array<int> my_array{};
    my_array.add(3);
    my_array.add(6);
    my_array.add(9);
    my_array.add(12);
    my_array.add(15);

    assert(my_array.find(12) == 3);
    assert(my_array.find(20) == -1);

    debugArray(my_array);
}

void testArrayClear() {
   pbrady::utils::structures::array<int> my_array{};
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
   pbrady::utils::structures::array<int> my_array{};
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
   pbrady::utils::structures::array<int> my_array{};
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
   pbrady::utils::structures::array<int> my_array{};
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
   pbrady::utils::structures::array<int> my_array = 
           pbrady::utils::structures::array<int>(&data[0], size);
    assert(my_array.size() == size);
    assert(my_array.capacity() == 2*size);
    
    assert(my_array.get(0) == data[0]);
    assert(my_array.get(1) == data[1]);
    assert(my_array.get(2) == data[2]);
}

void testSizeOnlyConstructor() {
    size_t size = 5;
    pbrady::utils::structures::array<int> my_array{size};
    assert(my_array.size() == size);
    assert(my_array.capacity() == 2*size);

    bool exception_happened = false;
    int val = -1;
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
   pbrady::utils::structures::array<int> my_array{};
    assert(my_array.size() == 0);
    assert(my_array.capacity() == 0);

    bool exception_happened = false;
    try {
        int val = my_array.get(1);
    } catch (const pbrady::utils::exception& e) {
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
}
