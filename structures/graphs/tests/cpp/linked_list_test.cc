#include "single_linked_list.hh"
#include "double_linked_list.hh"
#include "validator.hh"
#include "logger.hh"

#include <iostream>
#include <memory>
#include <map>

using namespace prb17::utils::structures;
static prb17::utils::logger logger{"linked_list_test"};

template<typename T>
void build_single_linked_list(prb17::utils::parsers::json_parser jp, single_linked_list<T> *sll) {
    // Construct the nodes list
    for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
        prb17::utils::parsers::json_parser tmp{jp.get_json_value()["graph"]["nodes"][i]};
            sll->add_node(tmp.as_string("id"), tmp.as_value<T>("value"));
    }
}

template<typename T>
void build_double_linked_list(prb17::utils::parsers::json_parser jp, double_linked_list<T> *dll) {
    // Construct the nodes list
    for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
        prb17::utils::parsers::json_parser tmp{jp.get_json_value()["graph"]["nodes"][i]};
            dll->add_node(tmp.as_string("id"), tmp.as_value<T>("value"));
    }
}

// template<typename T>
// void build_weighted_linked_list(prb17::utils::parsers::json_parser jp, weighted_graph<T> *g) {
//     // Construct the nodes list
//     for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
//         prb17::utils::parsers::json_parser tmp{jp.get_json_value()["graph"]["nodes"][i]};
//             weighted_vertex<T> *node = new weighted_vertex<T>(tmp.as_string("id"), 
//                                             tmp.as_value<T>("value"));
                                            
//             g->add_vertex(node);
//     }
//     // Assign all the edges for each node
//     for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
//         weighted_vertex<T> *node = g->get_vertex(jp.get_json_value()["graph"]["nodes"][i]["id"].asString());

//         std::string node_id = jp.get_json_value()["graph"]["nodes"][i]["next"].asString();
//         if (!node_id.empty()) {
//             weighted_vertex<T> *node_to_add = g->get_vertex(node_id);
//             g->add_edge_to_vertex(node, node_to_add);
//         }
//     }

//     vertex<T> *root = g->get_vertex(jp.get_json_value()["graph"]["root"].asString());
//     g->set_root(root);
// }

template<typename T>
bool single_linked_list_print(prb17::utils::parsers::json_parser jp) {
    logger.info("Building single linked list");
    single_linked_list<T> sll{};
    build_single_linked_list<T>(jp, &sll);
    logger.info("Created single linked list");
    logger.info("Calling single linked list to_string: \n\n{}\n", sll);
    logger.info("Calling single linked list adjacency_list \n\n{}", sll.to_adjacency_list());
    sll.cleanup();

    return true;
}

template<typename T>
bool double_linked_list_print(prb17::utils::parsers::json_parser jp) {
    logger.info("Building double linked list");
    double_linked_list<T> dll{};
    build_double_linked_list(jp, &dll);
    logger.info("Created double linked list");
    logger.info("Calling double linked list to_string: \n\n{}\n", dll);
    logger.info("Calling double linked list adjacency_list \n\n{}", dll.to_adjacency_list());
    dll.cleanup();

    return true;
}

//Map that relates the json file test config file to each test function defined in this file
template<typename T>
static std::map<std::string, std::function<bool(prb17::utils::parsers::json_parser)> > linked_list_tests = {
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
    
    validator.add_tests(&linked_list_tests<std::string>);
    validator.add_tests(&linked_list_tests<int>);
    validator.add_tests(&linked_list_tests<uint>);
    validator.add_tests(&linked_list_tests<char>);
    validator.add_tests(&linked_list_tests<bool>);
    validator.add_tests(&linked_list_tests<float>);
    validator.add_tests(&linked_list_tests<double>);

    logger.info("Starting validation tests of linked_list_tests");
    validator.validate();
    logger.info("Finished validation tests of linked_list_tests");
}
