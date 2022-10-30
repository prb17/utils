#include <iostream>

#include "stack.hh"

int main() {
    std::cout << "hello world" << std::endl;

    int *data = (int*)malloc(3*sizeof(int));
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;

    pbrady::structures::stack<int> my_stack = 
            pbrady::structures::stack<int>(data, 3);
    std::cout << "my stack: " << my_stack.to_string() << std::endl;

    std::cout << "my stack tail: " << my_stack.peek() << std::endl;

    free(data);
    data = nullptr;
}
