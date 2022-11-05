#include "graph.hh"
#include <iostream>

using namespace pbrady::utils::structures;

int main() {
    vertex<int> *node1 = new vertex<int>("node1", 5);
    // std::cout << *node1 << std::endl;

    vertex<int> *node2 = new vertex<int>("node2", 15);
    // std::cout << node2->to_string() << std::endl;

    vertex<int> *node3 = new vertex<int>("node3", 25);
    // std::cout << node3->to_string() << std::endl;

    node1->add_node( node2 );
    node1->add_node( node3 );
    // std::cout << *node1 << std::endl;

    auto g = graph<int>( node1 );

    //Get a feel for printing out graph
    std::cout << "calling graph's to_string operator: \n\n" << g << "" << std::endl;
}