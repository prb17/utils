#include "graph.hh"
#include "single_linked_list.hh"
#include "double_linked_list.hh"
#include "validator.hh"
#include "logger.hh"

#include <iostream>
#include <memory>
#include <map>

using namespace prb17::utils::structures;
static prb17::utils::logger logger{"graph_test"};

// TODO: Could this be better than going through the node list from config file twice?
template<typename T>
void build_graph(prb17::utils::parsers::json_parser jp, graph<T> *g) {
    // Construct the nodes list
    for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
        prb17::utils::parsers::json_parser tmp{jp.get_json_value()["graph"]["nodes"][i]};
            vertex<T> *node = new vertex<T>(tmp.as_string("id"), 
                                            tmp.as_value<T>("value"));
                                            
            g->add_vertex(node);
    }
    // Assign all the edges for each node
    for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
        vertex<T> *node = g->get_vertex(jp.get_json_value()["graph"]["nodes"][i]["id"].asString());

        for(Json::Value::ArrayIndex j=0; j < jp.get_json_value()["graph"]["nodes"][i]["edges"].size(); j++) {
            vertex<T> *node_to_add = g->get_vertex(jp.get_json_value()["graph"]["nodes"][i]["edges"][j].asString());
            g->add_edge_to_vertex(node, node_to_add);
        }
    }

    vertex<T> *root = g->get_vertex(jp.get_json_value()["graph"]["root"].asString());
    g->set_root(root);
}

template<typename T>
void build_weighted_graph(prb17::utils::parsers::json_parser jp, weighted_graph<T> *g) {
    // Construct the nodes list
    for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
        prb17::utils::parsers::json_parser tmp{jp.get_json_value()["graph"]["nodes"][i]};
            weighted_vertex<T> *node = new weighted_vertex<T>(tmp.as_string("id"), 
                                            tmp.as_value<T>("value"));
                                            
            g->add_vertex(node);
    }
    // Assign all the edges for each node
    for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
        weighted_vertex<T> *node = g->get_vertex(jp.get_json_value()["graph"]["nodes"][i]["id"].asString());

        for(Json::Value::ArrayIndex j=0; j < jp.get_json_value()["graph"]["nodes"][i]["edges"].size(); j++) {
            weighted_vertex<T> *node_to_add = g->get_vertex(jp.get_json_value()["graph"]["nodes"][i]["edges"][j]["id"].asString());
            g->add_edge_to_vertex(node, node_to_add);
        }
    }

    vertex<T> *root = g->get_vertex(jp.get_json_value()["graph"]["root"].asString());
    g->set_root(root);
}

//
template<typename T>
bool basic_graph_print(prb17::utils::parsers::json_parser jp) {
    logger.info("Building graph");
    graph<T> g{};
    build_graph<T>(jp, &g);

    logger.info("calling basic graph's to_string: \n\n{}", g);
    logger.info("calling graph's to_adjacency_list: \n\n{}", g.to_adjacency_list());
    logger.info("setting the graph's root node to Node '9'");
    g.set_root(g.get_vertex("9"));
    logger.info("calling basic graph's to_string: \n\n{}", g);
    g.cleanup();
    return true;
}

template<typename T>
bool weighted_graph_print(prb17::utils::parsers::json_parser jp) {
    logger.info("Building weighted graph");
    weighted_graph<T> g{};
    build_weighted_graph<T>(jp, &g);

    logger.info("calling weighted graph's to_string: \n\n{}", g);
    logger.info("calling weighted graph's to_adjacency_list: \n\n{}", g.to_adjacency_list());
    g.cleanup();

    return true;
}

template<typename T>
bool single_linked_list_print(prb17::utils::parsers::json_parser jp) {
    logger.info("Building single linked list");
    single_linked_list<T> sll{};
    build_graph<T>(jp, &sll);
    logger.info("Created single linked list");
    logger.info("Calling single linked list to_string: \n\n{}", sll);
    logger.info("Calling single linked list adjacency_list \n\n{}", sll.to_adjacency_list());
    sll.cleanup();

    return true;
}

template<typename T>
bool double_linked_list_print(prb17::utils::parsers::json_parser jp) {
    logger.info("Building double linked list");
    double_linked_list<T> dll{};
    logger.info("Created double linked list");
    dll.cleanup();

    return true;
}

//Map that relates the json file test config file to each test function defined in this file
template<typename T>
static std::map<std::string, std::function<bool(prb17::utils::parsers::json_parser)> > graph_tests = {
    {"basicGraphPrint", &basic_graph_print<T>},
    {"weightedGraphPrint", &weighted_graph_print<T>},
    {"singleLinkedListPrint", &single_linked_list_print<T>},
    {"doubleLinkedListPrint", &double_linked_list_print<T>}
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
    
    validator.add_tests(&graph_tests<std::string>);
    validator.add_tests(&graph_tests<int>);
    validator.add_tests(&graph_tests<uint>);
    validator.add_tests(&graph_tests<char>);
    validator.add_tests(&graph_tests<bool>);
    validator.add_tests(&graph_tests<float>);
    validator.add_tests(&graph_tests<double>);

    logger.info("Starting validation tests of graph_tests");
    validator.validate();
    logger.info("Finished validation tests of graph_tests");
}
