#include <iostream>
#include <cassert>

#include "stack.hh"

void debugStack(prb17::utils::structures::stack<int>& stack) {
    std::cout << stack << std::endl;
}

// void testOperators() {
//    prb17::utils::structures::stack<int> stack1{};
//     stack1.push(3);
//     stack1.push(6);
//     stack1.push(9);
//     stack1.push(12);
//     stack1.push(15);
//     std::cout << "stack1: ";
//     debugStack(stack1);

//     prb17::utils::structures::stack<int> stack2{};
//     assert(stack2 != stack1);

//     stack2 = stack1;
//     std::cout << "stack2: ";
//     debugStack(stack2);
//     assert(stack1 == stack2);

//     assert(stack1[3] == stack2[3]);

//     stack1[3] = 4;
//     std::cout << "stack1: ";
//     debugStack(stack1);
//     assert(stack1[3] != stack2[3]);
//     assert(stack1 != stack2);

//     stack2[3] = stack1[3];
//     assert(stack1 == stack2); 
// }

// void testPush() {
//     std::cout << "Testing stack push method" << std::endl;
//    prb17::utils::structures::stack<int> my_stack{};

//    my_stack.push(1);
//    assert(my_stack.peek() == 1);
//    debugStack(my_stack);

//    my_stack.push(10);
//    assert(my_stack.peek() == 10);
//    debugStack(my_stack);

//    my_stack.push(12345);
//    assert(my_stack.peek() == 12345);
//    debugStack(my_stack);

//    my_stack.push(-1);
//    assert(my_stack.peek() == -1);
//    debugStack(my_stack);

//    my_stack.push(INT32_MAX);
//    assert(my_stack.peek() == INT32_MAX);
//    debugStack(my_stack);
   
//    my_stack.push(UINT32_MAX);
//    assert(my_stack.peek() == UINT32_MAX);
//    debugStack(my_stack);

//    std::cout << "Finished testing stack push method" << std::endl;
// }

// void testPop() {
//     std::cout << "Testing stack pop method" << std::endl;
//    prb17::utils::structures::stack<int> my_stack{};

//    my_stack.push(1);
//    debugStack(my_stack);
//    my_stack.push(10);
//    debugStack(my_stack);
//    my_stack.push(12345);
//    debugStack(my_stack);
//    my_stack.push(-1);
//    debugStack(my_stack);
//    my_stack.push(INT32_MAX);
//    debugStack(my_stack);
//    my_stack.push(UINT32_MAX);
//    debugStack(my_stack);
//    assert(my_stack.size() == 6);
//    debugStack(my_stack);

   
//    assert(my_stack.peek() == UINT32_MAX);
//    my_stack.pop();
//    debugStack(my_stack);
//    assert(my_stack.peek() == INT32_MAX);  
//    my_stack.pop();
//    debugStack(my_stack);
//    assert(my_stack.peek() == -1);
//    my_stack.pop();
//    debugStack(my_stack);
//    assert(my_stack.peek() == 12345);
//    my_stack.pop();
//    debugStack(my_stack);
//    assert(my_stack.pop() == 10);
//    debugStack(my_stack);
   
//    assert(my_stack.pop() == 1);
//    assert(my_stack.size() == 0);
//    debugStack(my_stack);

//    std::cout << "Finished testing stack pop method" << std::endl;
// }

// void testSizeOnlyConstructor() {
//     size_t size = 5;
//     prb17::utils::structures::stack<int> my_stack{size};
//     assert(my_stack.size() == size);
//     assert(my_stack.capacity() == 2*size);

//     bool exception_happened = false;
//     int val = -1;
//     try {
//         val = my_stack.peek();
//     } catch (const std::exception& e) {
//         exception_happened = true;
//     }
//     assert(!exception_happened);
//     assert(val == 0);    
// }

// void testDefaultConstructor() {
//    prb17::utils::structures::stack<int> my_stack{};
//     assert(my_stack.size() == 0);
//     assert(my_stack.capacity() == 0);

//     bool exception_happened = false;
//     try {
//         if (my_stack.size()) {
//             int val = my_stack.peek();
//         }
        
//     } catch (const std::exception& e) {
//         exception_happened = true;
//     }
//     assert(!exception_happened);

// }

int main() {
    // testDefaultConstructor();
    // testSizeOnlyConstructor();

    // testPush();
    // testPop();

    // testOperators();
}
