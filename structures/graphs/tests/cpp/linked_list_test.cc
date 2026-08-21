#include <iostream>
#include <string>

#include "graph.hh"
#include "vertex.hh"
#include "validator.hh"
#include "logger.hh"

#include "traverse.hh"
#include "graph_test_helper.hh"

// A linked list is just a graph: a single linked list is a chain of "next"
// edges, and a double linked list adds the matching "prev" edges (which
// introduce cycles). These tests exercise the graph algorithms against those
// shapes and assert real, config-driven expectations.

using namespace prb17::utils::structures;
namespace algo = prb17::utils::algorithms::graphs;

static prb17::utils::logger logger{"linked_list_test"};

template<typename T>
static array<std::string> ids_of(array<vertex<T>*> nodes) {
    array<std::string> ids{};
    for (size_t i=0; i<nodes.size(); i++) {
        ids.add(nodes[i]->get_id());
    }
    return ids;
}

// a single linked list traverses head->tail in chain order (dfs == bfs here),
// and holds the expected number of nodes
template<typename T>
bool testSingleListOrder(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_graph_from_config<T>(jp);

    int expected_count = jp.as_int("expected_count");
    std::string head_id = jp.as_string("head_id");
    array<std::string> expected_order = jp.as_string_array("expected_order");

    array<std::string> dfs_order = ids_of(algo::dfs(*g, head_id));
    array<std::string> bfs_order = ids_of(algo::bfs(*g, head_id));
    logger.debug("single list dfs: {}, bfs: {}, expected: {}", dfs_order, bfs_order, expected_order);

    bool result = (int)g->get_count() == expected_count
                  && dfs_order == expected_order
                  && bfs_order == expected_order;

    g->cleanup();
    delete g;
    return result;
}

// a double linked list has back-edges (cycles); traversal must still terminate
// and visit each reachable node exactly once
template<typename T>
bool testDoubleListTerminates(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_graph_from_config<T>(jp);

    std::string start_id = jp.as_string("start_id");
    int expected_visited = jp.as_int("expected_visited");
    int dfs_count = algo::dfs(*g, start_id).size();
    int bfs_count = algo::bfs(*g, start_id).size();
    logger.debug("double list dfs visited '{}', bfs visited '{}', expected '{}'", dfs_count, bfs_count, expected_visited);

    bool result = dfs_count == expected_visited && bfs_count == expected_visited;

    g->cleanup();
    delete g;
    return result;
}

// the back-edges of a double linked list make the tail reach the head; dfs from
// the tail walks the list in reverse
template<typename T>
bool testDoubleListReverseOrder(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_graph_from_config<T>(jp);

    std::string tail_id = jp.as_string("tail_id");
    array<std::string> expected_order = jp.as_string_array("expected_order");
    array<std::string> got = ids_of(algo::dfs(*g, tail_id));
    logger.debug("reverse dfs from tail got: {}, expected: {}", got, expected_order);

    bool result = got == expected_order;

    g->cleanup();
    delete g;
    return result;
}

// the adjacency list names every node and annotates each link with its weight
template<typename T>
bool testLinkedListAdjacency(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_graph_from_config<T>(jp);

    std::string rendered = algo::to_adjacency_list(*g);
    array<std::string> expected_ids = jp.as_string_array("expected_ids");
    bool result = true;
    for (size_t i=0; i<expected_ids.size(); i++) {
        result &= rendered.find(expected_ids[i]) != std::string::npos;
    }
    result &= rendered.find("(w:") != std::string::npos;
    logger.debug("linked list adjacency:\n{}\ncontains all ids + weights: '{}'", rendered, result);

    g->cleanup();
    delete g;
    return result;
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;

    tests.add(prb17::utils::test{"testSingleListOrder", &testSingleListOrder<T>});
    tests.add(prb17::utils::test{"testDoubleListTerminates", &testDoubleListTerminates<T>});
    tests.add(prb17::utils::test{"testDoubleListReverseOrder", &testDoubleListReverseOrder<T>});
    tests.add(prb17::utils::test{"testLinkedListAdjacency", &testLinkedListAdjacency<T>});

    return tests;
}
template<typename T>
static prb17::utils::structures::array<prb17::utils::test> linked_list_tests = build_tests<T>();

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

    validator.add_tests(linked_list_tests<int>);

    logger.info("Starting validation tests of linked_list_tests");
    validator.validate();
    logger.info("Finished validation tests of linked_list_tests");
}
