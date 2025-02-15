#include "graph.hh"
#include "validator.hh"
#include "logger.hh"

#include "structures_director.hh"
#include "graph_builder.hh"

#include <iostream>
#include <memory>
#include <map>

using namespace prb17::utils::structures;
static prb17::utils::logger logger{"graph_test"};

// TODO: Could this be better than going through the node list from config file twice?
template<typename T>
graph<T>* build_graph(prb17::utils::parsers::json_parser jp) {
    structures_director d{};
    graph_builder<T> b{};

    // Construct the nodes list
    for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
        prb17::utils::parsers::json_parser tmp{jp.get_json_value()["graph"]["nodes"][i]};
        b.add(tmp.as_string("id"), tmp.as_value<T>("value"), tmp.as_string_array("edges"));
    }

    d.construct(&b);
    return b.graph_product();
}

template<typename T>
graph<T>* build_weighted_graph(prb17::utils::parsers::json_parser jp) {
    structures_director d{};
    graph_builder<T> b{};

    // Assign all the edges for each node
    for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
        prb17::utils::parsers::json_parser tmp{jp.get_json_value()["graph"]["nodes"][i]};
        prb17::utils::structures::array<std::string> tmp_edges{};
        prb17::utils::structures::array<int> tmp_weights{};
        for(Json::Value::ArrayIndex j=0; j < jp.get_json_value()["graph"]["nodes"][i]["edges"].size(); j++) {
            std::string e = jp.get_json_value()["graph"]["nodes"][i]["edges"][j]["id"].asString();
            int w = jp.get_json_value()["graph"]["nodes"][i]["edges"][j]["weight"].asInt();
            tmp_edges.add(e);
            tmp_weights.add(w);
        }
        b.add(tmp.as_string("id"), tmp.as_value<T>("value"), tmp_edges, tmp_weights);
    }

    d.construct(&b);
    return b.graph_product();
}

//
template<typename T>
bool basic_graph_print(prb17::utils::parsers::json_parser jp) {
    logger.info("Building graph");
    
    graph<T> *g = build_graph<T>(jp);

    logger.info("calling basic graph's to_string: \n\n{}", g);
    logger.info("calling graph's to_adjacency_list: \n\n{}", g->to_adjacency_list());

    g->cleanup();
    delete g;
    return true;
}

template<typename T>
bool weighted_graph_print(prb17::utils::parsers::json_parser jp) {
    logger.info("Building weighted graph");
    graph<T> *wg = build_weighted_graph<T>(jp);

    logger.info("calling weighted graph's to_string: \n\n{}", wg);
    logger.info("calling weighted graph's to_adjacency_list: \n\n{}", wg->to_adjacency_list());

    wg->cleanup();
    delete wg;
    return true;
}

//Map that relates the json file test config file to each test function defined in this file
template<typename T>
static std::map<std::string, std::function<bool(prb17::utils::parsers::json_parser)> > graph_tests = {
    {"basicGraphPrint", &basic_graph_print<T>},
    {"weightedGraphPrint", &weighted_graph_print<T>}
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
    
    structures_director d{};
    graph_builder<int> b = graph_builder<int>{};
    b.add("100", 10)
     .add("101", 45)
     .add("102", 78)
     .add_edge_to_vertex("100", "101")
     .add_edge_to_vertex("102", "100")
     .add_edge_to_vertex("101", "100")
     .add_edge_to_vertex("101", "102");
    d.construct(&b);
    graph<int> *g = b.graph_product();

    logger.info("Printing to_string when manual building of graph in typical builder fashion: \n'\n{}'\n", g);
    logger.info("Printing adjacency list when manual building of graph in typical builder fashion: \n'\n{}'\n", g->to_adjacency_list());
    g->cleanup();
    delete g;
}
