#include <iostream>
#include <cassert>

#include "queue.hh"


#include<vector>

void debug_queue(pbrady::utils::structures::queue<int> &queue) {
    std::cout << queue.to_string() << std::endl;
}

void test_enqueue() {
    std::cout << "starting enqueue test" << std::endl; 

    pbrady::utils::structures::queue<int> my_queue{};
    debug_queue(my_queue);
    my_queue.enqueue(4);
    debug_queue(my_queue);
    assert(my_queue.get_size() == 1);
    assert(my_queue.get_capacity() == 2);
    assert(my_queue.front() == 4);
    assert(my_queue.back() == 4);
    
    my_queue.enqueue(10);
    debug_queue(my_queue);
    assert(my_queue.back() == 10);
    assert(my_queue.front() == 4);

    std::cout << "finished enqueue test" << std::endl;
}

void test_dequeue() {    
    std::cout << "starting dequeue test" << std::endl;

    pbrady::utils::structures::queue<int> my_queue{};
    my_queue.enqueue(4);
    my_queue.enqueue(12);
    my_queue.enqueue(345);
    my_queue.enqueue(-45);
    my_queue.enqueue(0);
    my_queue.enqueue(1);
    my_queue.enqueue(-1);
    debug_queue(my_queue);
    assert(my_queue.get_size() == 7);

    assert(my_queue.back() == -1);
    assert(my_queue.front() == 4);
    my_queue.dequeue();
    debug_queue(my_queue);
    assert(my_queue.back() == -1);
    assert(my_queue.front() == 12);
    my_queue.dequeue();
    debug_queue(my_queue);
    assert(my_queue.back() == -1);
    assert(my_queue.front() == 345);
    my_queue.dequeue();
    debug_queue(my_queue);
    assert(my_queue.back() == -1);
    assert(my_queue.front() == -45);
    my_queue.dequeue();
    debug_queue(my_queue);
    assert(my_queue.back() == -1);
    assert(my_queue.front() == 0);
    my_queue.dequeue();
    debug_queue(my_queue);
    
    my_queue.enqueue(-18);

    assert(my_queue.back() == -18);
    assert(my_queue.front() == 1);
    my_queue.dequeue();
    debug_queue(my_queue);

    assert(my_queue.back() == -18);
    assert(my_queue.front() == -1);
    my_queue.dequeue();
    debug_queue(my_queue);
    
    assert(my_queue.back() == -18);
    assert(my_queue.front() == -18);
    my_queue.dequeue();
    debug_queue(my_queue);

    assert(my_queue.get_size() == 0);

    std::cout << "finished dequeue test" << std::endl;
}

void test_alternate_enq_deq() {
    std::cout << "starting alternating test" << std::endl;

    pbrady::utils::structures::queue<int> my_queue{};
    my_queue.enqueue(4);
    my_queue.enqueue(12);
    my_queue.enqueue(345);
    my_queue.enqueue(-45);
    my_queue.enqueue(0);
    my_queue.enqueue(1);
    my_queue.enqueue(-1);
    debug_queue(my_queue);
    assert(my_queue.get_size() == 7);

    assert(my_queue.back() == -1);
    assert(my_queue.front() == 4);
    my_queue.dequeue();
    debug_queue(my_queue);

    my_queue.enqueue(34);
    assert(my_queue.back() == 34);
    assert(my_queue.front() == 12);
    my_queue.dequeue();
    debug_queue(my_queue);
    
    my_queue.enqueue(56);
    assert(my_queue.back() == 56);
    assert(my_queue.front() == 345);
    my_queue.dequeue();
    debug_queue(my_queue);
    
    my_queue.enqueue(987);
    assert(my_queue.back() == 987);
    assert(my_queue.front() == -45);
    my_queue.dequeue();
    debug_queue(my_queue);
    
    my_queue.enqueue(213);
    assert(my_queue.back() == 213);
    assert(my_queue.front() == 0);
    my_queue.dequeue();
    debug_queue(my_queue);
    
    my_queue.enqueue(-18);
    assert(my_queue.back() == -18);
    assert(my_queue.front() == 1);
    my_queue.dequeue();
    debug_queue(my_queue);

    my_queue.enqueue(-81);
    assert(my_queue.back() == -81);
    assert(my_queue.front() == -1);
    my_queue.dequeue();
    debug_queue(my_queue);
    
    my_queue.enqueue(0);
    assert(my_queue.back() == 0);
    assert(my_queue.front() == 34);
    my_queue.dequeue();
    debug_queue(my_queue);

    assert(my_queue.get_size() == 6);

    std::cout << "finished alternating test" << std::endl;
}

int main() {
    test_enqueue();
    test_dequeue();
    test_alternate_enq_deq();
}
