#include <iostream>
#include <cassert>

#include "stack.hh"

void testPush() {
   pbrady::utils::structures::stack<int> my_stack{};

   my_stack.push(1);
   assert(my_stack.peek() == 1);

   my_stack.push(10);
   assert(my_stack.peek() == 10);

   my_stack.push(12345);
   assert(my_stack.peek() == 12345);

   my_stack.push(-1);
   assert(my_stack.peek() == -1);

   my_stack.push(INT32_MAX);
   assert(my_stack.peek() == INT32_MAX);
   
   my_stack.push(UINT32_MAX);
   assert(my_stack.peek() == UINT32_MAX);
}

void testPop() {
   pbrady::utils::structures::stack<int> my_stack{};

   my_stack.push(1);
   my_stack.push(10);
   my_stack.push(12345);
   my_stack.push(-1);
   my_stack.push(INT32_MAX);
   my_stack.push(UINT32_MAX);
   assert(my_stack.get_size() == 6);

   
   assert(my_stack.peek() == UINT32_MAX);
   my_stack.pop();
   assert(my_stack.peek() == INT32_MAX);  
   my_stack.pop();
   assert(my_stack.peek() == -1);
   my_stack.pop();
   assert(my_stack.peek() == 12345);
   my_stack.pop();
   assert(my_stack.peek() == 10);
   my_stack.pop();
   assert(my_stack.peek() == 1);
   my_stack.pop();
   assert(my_stack.get_size() == 0);

}

void testFullArrayConstructor() {
    size_t size = 3;
    int data[] = {1, 2, 3};
   pbrady::utils::structures::stack<int> my_stack = 
           pbrady::utils::structures::stack<int>(&data[0], size);
    assert(my_stack.get_size() == size);
    assert(my_stack.get_capacity() == 2*size);
}

void testSizeOnlyConstructor() {
    size_t size = 5;
    pbrady::utils::structures::stack<int> my_stack{size};
    assert(my_stack.get_size() == size);
    assert(my_stack.get_capacity() == 2*size);

    bool exception_happened = false;
    int val = -1;
    try {
        val = my_stack.peek();
    } catch (const std::exception& e) {
        exception_happened = true;
    }
    assert(exception_happened);
    assert(val == 0);    
}

void testDefaultConstructor() {
   pbrady::utils::structures::stack<int> my_stack{};
    assert(my_stack.get_size() == 0);
    assert(my_stack.get_capacity() == 0);

    bool exception_happened = false;
    try {
        int val = my_stack.peek();
    } catch (const std::exception& e) {
        exception_happened = true;
    }
    assert(exception_happened);

}

int main() {
    testDefaultConstructor();
    testSizeOnlyConstructor();

    testPush();
    testPop();
}
