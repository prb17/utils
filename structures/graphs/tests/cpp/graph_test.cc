#include "graph.hh"
#include <iostream>
#include <memory>

using namespace prb17::utils::structures;

void unique_ptr_graph_print() {
    std::unique_ptr<vertex<int>> node1{new vertex<int>("node1", 6)};
    std::unique_ptr<vertex<int>> node2{new vertex<int>("node2", 12)};
    std::unique_ptr<vertex<int>> node3{new vertex<int>("node3", 18)};    

    node1->add_edge( node2.get() );
    node1->add_edge( node3.get() );

    auto g = graph<int>( node1.get() );

    //Get a feel for printing out graph
    std::cout << "calling basic graph's to_string operator: \n\n" << g << "" << std::endl;
}

void basic_graph_print() {
    vertex<int> *node1 = new vertex<int>("node1", 5);
    // std::cout << *node1 << std::endl;

    vertex<int> *node2 = new vertex<int>("node2", 15);
    // std::cout << node2->to_string() << std::endl;

    vertex<int> *node3 = new vertex<int>("node3", 25);
    // std::cout << node3->to_string() << std::endl;

    node1->add_edge( node2 );
    node1->add_edge( node3 );
    // std::cout << *node1 << std::endl;

    auto g = graph<int>( node1 );

    //Get a feel for printing out graph
    std::cout << "calling basic graph's to_string operator: \n\n" << g << "" << std::endl;
    delete node1;
    delete node2;
    delete node3;
}

void weighted_graph_print() {    
    vertex<int> *node1 = new vertex<int>("node1", 3);
    vertex<int> *node2 = new vertex<int>("node2", 9);
    vertex<int> *node3 = new vertex<int>("node3", 12);
    vertex<int> *node4 = new vertex<int>("node4", 1);
    vertex<int> *node5 = new vertex<int>("node5", 5);
    vertex<int> *node6 = new vertex<int>("node6", 7);
    vertex<int> *node7 = new vertex<int>("node7", 162);
    vertex<int> *node8 = new vertex<int>("node8", 6);
    vertex<int> *node9 = new vertex<int>("node9", 2);
    vertex<int> *node10 = new vertex<int>("node10", 45);

    node1->add_edge(node3, 5);
    node1->add_edge(node2, 10);

    node3->add_edge(node4);
    node3->add_edge(node5);
    node3->add_edge(node6);

    node2->add_edge(node7);
    node2->add_edge(node8);
    node2->add_edge(node9);
    node2->add_edge(node10);

    auto g = graph<int>( node1 );

    //Get a feel for printing out graph
    std::cout << "calling weighted graph's to_string operator: \n\n" << g << "" << std::endl;
    delete node1;
    delete node2;
    delete node3;
    delete node4;
    delete node5;
    delete node6;
    delete node7;
    delete node8;
    delete node9;
    delete node10;
}

int main() {
    basic_graph_print();
    weighted_graph_print();
    unique_ptr_graph_print();
}