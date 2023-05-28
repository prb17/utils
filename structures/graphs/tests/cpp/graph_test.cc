#include "graph.hh"
#include "validator.hh"
#include "logger.hh"

#include <iostream>
#include <memory>
#include <map>

using namespace prb17::utils::structures;
static prb17::utils::logger logger{"graph_test"};

// TODO: Could this be better than going through the node list from config file twice?
// Also, I know there's memory leaks for now, but address that later
// Also, getting the 'value' won't always be an integer, I'll have to find a way to get a multi-nested value generically a.k.a. jp['graph']['nodes']['value']<T> or something to that affect
template<typename T>
graph<T> build_graph(prb17::utils::parsers::json_parser jp) {
    std::map<std::string, vertex<T>*> nodes_list{};
    // Construct the nodes list
    for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
            vertex<T> *node = new vertex<T>(jp.get_json_value()["graph"]["nodes"][i]["id"].asString(), 
                                            jp.get_json_value()["graph"]["nodes"][i]["value"].asInt());
                                            
            nodes_list.emplace(node->get_id(), node);
    }

    // Assign all the edges for each node
    for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
        vertex<T> *node = nodes_list[jp.get_json_value()["graph"]["nodes"][i]["id"].asString()];

        for(Json::Value::ArrayIndex j=0; j < jp.get_json_value()["graph"]["nodes"][i]["edges"].size(); j++) {
            vertex<T> *node_to_add = nodes_list[jp.get_json_value()["graph"]["nodes"][i]["edges"][j].asString()];
            node->add_edge(node_to_add);
        }
    }

    vertex<T> *root = nodes_list[jp.get_json_value()["graph"]["root"].asString()];
    return graph<T>( root );
}

template<typename T>
bool unique_ptr_graph_print(prb17::utils::parsers::json_parser jp) {
    std::unique_ptr<vertex<int>> node1{new vertex<int>("node1", 6)};
    std::unique_ptr<vertex<int>> node2{new vertex<int>("node2", 12)};
    std::unique_ptr<vertex<int>> node3{new vertex<int>("node3", 18)};    

    node1->add_edge( node2.get() );
    node1->add_edge( node3.get() );

    auto g = graph<int>( node1.get() );

    //Get a feel for printing out graph
    std::cout << "calling basic graph's to_string operator: \n\n" << g << "" << std::endl;
    return true;
}

template<typename T>
bool basic_graph_print(prb17::utils::parsers::json_parser jp) {
    // vertex<int> *node1 = new vertex<int>(jp.get_json_value()["graph"]["nodes"][0]["id"].asString(), 
    //                                         jp.get_json_value()["graph"]["nodes"][0]["value"].asInt());
    // // std::cout << *node1 << std::endl;

    // vertex<int> *node2 = new vertex<int>("node2", 15);
    // // std::cout << node2->to_string() << std::endl;

    // vertex<int> *node3 = new vertex<int>("node3", 25);
    // // std::cout << node3->to_string() << std::endl;

    // node1->add_edge( node2 );
    // node1->add_edge( node3 );
    // std::cout << *node1 << std::endl;
    logger.info("Building graph");
    graph<T> g = build_graph<T>(jp);

    //Get a feel for printing out graph
    logger.info("calling basic graph's to_string: \n\n{}", g);
    // delete node1;
    // delete node2;
    // delete node3;
    return true;
}

template<typename T>
bool weighted_graph_print(prb17::utils::parsers::json_parser jp) {    
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
    return true;
}

//Map that relates the json file test config file to each test function defined in this file
template<typename T>
static std::map<std::string, std::function<bool(prb17::utils::parsers::json_parser)> > graph_tests = {
    {"basicGraphPrint", &basic_graph_print<T>},
    {"weightedGraphPrint", &weighted_graph_print<T>},
    {"uniquePtrGraphPrint", &unique_ptr_graph_print<T>}
};

#define MIN_NUM_ARGS 2
int main(int argc, char** argv) {
    if (argc < MIN_NUM_ARGS) {
        throw prb17::utils::exception("This test requires a config file to be provided");
    }
    prb17::utils::structures::array<std::string> test_files{};
    for (int i=1; i<argc; i++) {
        //TODO: test if string is a valid file name/path
        logger.debug("adding test file to validator: '{}'", &argv[i][0]);
        test_files.add(&argv[i][0]);
    }
    prb17::utils::validator validator{test_files};
    
    // validator.add_tests(&graph_tests<std::string>);
    validator.add_tests(&graph_tests<int>);
    // validator.add_tests(&graph_tests<uint>);
    // validator.add_tests(&graph_tests<char>);
    // validator.add_tests(&graph_tests<bool>);
    // validator.add_tests(&graph_tests<float>);
    // validator.add_tests(&graph_tests<double>);

    logger.info("Starting validation tests of graph_tests");
    validator.validate();
    logger.info("Finished validation tests of graph_tests");
}
