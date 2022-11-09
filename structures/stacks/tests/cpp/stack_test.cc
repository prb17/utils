#include <iostream>
#include <cassert>

#include "stack.hh"

void debugStack(pbrady::utils::structures::stack<int>& stack) {
    std::cout << stack << std::endl;
}

void testPush() {
    std::cout << "Testing stack push method" << std::endl;
   pbrady::utils::structures::stack<int> my_stack{};

   my_stack.push(1);
   assert(my_stack.peek() == 1);
   debugStack(my_stack);

   my_stack.push(10);
   assert(my_stack.peek() == 10);
   debugStack(my_stack);

   my_stack.push(12345);
   assert(my_stack.peek() == 12345);
   debugStack(my_stack);

   my_stack.push(-1);
   assert(my_stack.peek() == -1);
   debugStack(my_stack);

   my_stack.push(INT32_MAX);
   assert(my_stack.peek() == INT32_MAX);
   debugStack(my_stack);
   
   my_stack.push(UINT32_MAX);
   assert(my_stack.peek() == UINT32_MAX);
   debugStack(my_stack);

   std::cout << "Finished testing stack push method" << std::endl;
}

void testPop() {
    std::cout << "Testing stack pop method" << std::endl;
   pbrady::utils::structures::stack<int> my_stack{};

   my_stack.push(1);
   debugStack(my_stack);
   my_stack.push(10);
   debugStack(my_stack);
   my_stack.push(12345);
   debugStack(my_stack);
   my_stack.push(-1);
   debugStack(my_stack);
   my_stack.push(INT32_MAX);
   debugStack(my_stack);
   my_stack.push(UINT32_MAX);
   debugStack(my_stack);
   assert(my_stack.get_size() == 6);
   debugStack(my_stack);

   
   assert(my_stack.peek() == UINT32_MAX);
   my_stack.pop();
   debugStack(my_stack);
   assert(my_stack.peek() == INT32_MAX);  
   my_stack.pop();
   debugStack(my_stack);
   assert(my_stack.peek() == -1);
   my_stack.pop();
   debugStack(my_stack);
   assert(my_stack.peek() == 12345);
   my_stack.pop();
   debugStack(my_stack);
   assert(my_stack.peek() == 10);
   my_stack.pop();
   debugStack(my_stack);
   assert(my_stack.peek() == 1);
   my_stack.pop();
   assert(my_stack.get_size() == 0);
   debugStack(my_stack);

   std::cout << "Finished testing stack pop method" << std::endl;
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
    assert(!exception_happened);
    assert(val == 0);    
}

void testDefaultConstructor() {
   pbrady::utils::structures::stack<int> my_stack{};
    assert(my_stack.get_size() == 0);
    assert(my_stack.get_capacity() == 0);

    bool exception_happened = false;
    try {
        if (my_stack.get_size()) {
            int val = my_stack.peek();
        }
        
    } catch (const std::exception& e) {
        exception_happened = true;
    }
    assert(!exception_happened);

}

int main() {
    testDefaultConstructor();
    testSizeOnlyConstructor();

    testPush();
    testPop();
}
