#include <iostream>
#include <string>

#include "graph.hh"
#include "vertex.hh"
#include "validator.hh"
#include "logger.hh"

#include "traverse.hh"
#include "graph_test_helper.hh"

using namespace prb17::utils::structures;
namespace algo = prb17::utils::algorithms::graphs;

static prb17::utils::logger logger{"graph_test"};

// Collect the ids of a traversal result into an array<std::string> so it can be
// compared directly against an expected id list read from the json config.
template<typename T>
static array<std::string> ids_of(array<vertex<T>*> nodes) {
    array<std::string> ids{};
    for (size_t i=0; i<nodes.size(); i++) {
        ids.add(nodes[i]->get_id());
    }
    return ids;
}

// number of vertices stored matches the configured expectation
template<typename T>
bool testNodeCount(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_weighted_graph_from_config<T>(jp);

    int expected = jp.as_int("expected_count");
    int result = g->get_count();
    logger.debug("expected count: '{}', result count: '{}'", expected, result);

    g->cleanup();
    delete g;
    return expected == result;
}

// get(id) returns the vertex carrying that id (and value)
template<typename T>
bool testGetById(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_weighted_graph_from_config<T>(jp);

    std::string lookup_id = jp.as_string("lookup_id");
    T expected_value = jp.as_value<T>("expected_value");
    vertex<T>* v = g->get(lookup_id);

    bool result = (v != nullptr) && (v->get_id() == lookup_id) && (v->get() == expected_value);
    logger.debug("lookup id '{}', found: '{}'", lookup_id, result);

    g->cleanup();
    delete g;
    return result;
}

// get(id) for an id not in the graph returns nullptr
template<typename T>
bool testGetMissingReturnsNull(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_weighted_graph_from_config<T>(jp);

    std::string lookup_id = jp.as_string("lookup_id");
    bool result = g->get(lookup_id) == nullptr;
    logger.debug("missing id '{}' returned nullptr: '{}'", lookup_id, result);

    g->cleanup();
    delete g;
    return result;
}

// adding a vertex whose id already exists is rejected and does not bump count
template<typename T>
bool testDuplicateAddRejected(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_weighted_graph_from_config<T>(jp);

    std::string dup_id = jp.as_string("dup_id");
    size_t before = g->get_count();
    vertex<T>* dup = new vertex<T>{dup_id, jp.as_value<T>("value")};
    bool added = g->add(dup);
    size_t after = g->get_count();

    bool result = (added == false) && (before == after);
    logger.debug("duplicate add returned: '{}', count before: '{}', after: '{}'", added, before, after);

    delete dup; // rejected, so ownership never transferred to the graph
    g->cleanup();
    delete g;
    return result;
}

// depth-first preorder from start_id matches the configured order
template<typename T>
bool testDfsOrder(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_weighted_graph_from_config<T>(jp);

    std::string start_id = jp.as_string("start_id");
    array<std::string> got = ids_of(algo::dfs(*g, start_id));
    array<std::string> expected = jp.as_string_array("expected_order");
    logger.debug("dfs got: {}, expected: {}", got, expected);

    g->cleanup();
    delete g;
    return got == expected;
}

// breadth-first level order from start_id matches the configured order
template<typename T>
bool testBfsOrder(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_weighted_graph_from_config<T>(jp);

    std::string start_id = jp.as_string("start_id");
    array<std::string> got = ids_of(algo::bfs(*g, start_id));
    array<std::string> expected = jp.as_string_array("expected_order");
    logger.debug("bfs got: {}, expected: {}", got, expected);

    g->cleanup();
    delete g;
    return got == expected;
}

// cycles and self-loops must terminate and visit each reachable node exactly once
template<typename T>
bool testCycleTermination(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_weighted_graph_from_config<T>(jp);

    std::string start_id = jp.as_string("start_id");
    int expected = jp.as_int("expected_visited");
    int dfs_count = algo::dfs(*g, start_id).size();
    int bfs_count = algo::bfs(*g, start_id).size();
    logger.debug("cycle: dfs visited '{}', bfs visited '{}', expected '{}'", dfs_count, bfs_count, expected);

    g->cleanup();
    delete g;
    return dfs_count == expected && bfs_count == expected;
}

// to_string spans every vertex, including nodes in disconnected components
template<typename T>
bool testToStringContainsAll(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_weighted_graph_from_config<T>(jp);

    std::string rendered = algo::to_string(*g);
    array<std::string> expected_ids = jp.as_string_array("expected_ids");
    bool result = true;
    for (size_t i=0; i<expected_ids.size(); i++) {
        result &= rendered.find(expected_ids[i]) != std::string::npos;
    }
    logger.debug("to_string:\n{}\ncontains all ids: '{}'", rendered, result);

    g->cleanup();
    delete g;
    return result;
}

// an empty graph renders to an empty string and does not throw
template<typename T>
bool testEmptyGraphRenders(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_weighted_graph_from_config<T>(jp);

    bool expected_empty = jp.as_bool("expected_empty");
    std::string rendered = algo::to_string(*g);
    bool result = rendered.empty() == expected_empty;
    logger.debug("empty graph rendered length '{}', expected_empty '{}'", rendered.size(), expected_empty);

    g->cleanup();
    delete g;
    return result;
}

// adjacency list names every vertex and annotates edges with weights
template<typename T>
bool testAdjacencyList(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_weighted_graph_from_config<T>(jp);

    std::string rendered = algo::to_adjacency_list(*g);
    array<std::string> expected_ids = jp.as_string_array("expected_ids");
    bool result = true;
    for (size_t i=0; i<expected_ids.size(); i++) {
        result &= rendered.find(expected_ids[i]) != std::string::npos;
    }
    // weights must be annotated on the edges
    result &= rendered.find("(w:") != std::string::npos;
    logger.debug("adjacency list:\n{}\ncontains all ids + weights: '{}'", rendered, result);

    g->cleanup();
    delete g;
    return result;
}

// weights written on edges can be read back in the same order
template<typename T>
bool testWeightReadback(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_weighted_graph_from_config<T>(jp);

    std::string node_id = jp.as_string("node_id");
    vertex<T>* v = g->get(node_id);
    array<int> got{};
    for (size_t i=0; i<v->num_edges(); i++) {
        got.add(v->get_weight(i));
    }
    array<int> expected = jp.as_int_array("expected_weights");
    logger.debug("weights got: {}, expected: {}", got, expected);

    g->cleanup();
    delete g;
    return got == expected;
}

// removing an edge keeps the parallel weights array aligned with the edges
template<typename T>
bool testRemoveEdgeKeepsWeightsAligned(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_weighted_graph_from_config<T>(jp);

    std::string node_id = jp.as_string("node_id");
    size_t remove_idx = jp.as_int("remove_idx");
    vertex<T>* v = g->get(node_id);
    bool removed = v->remove_edge(remove_idx);

    array<std::string> got_ids{};
    array<int> got_weights{};
    for (size_t i=0; i<v->num_edges(); i++) {
        got_ids.add(v->get_connected_vertex(i)->get_id());
        got_weights.add(v->get_weight(i));
    }
    array<std::string> expected_ids = jp.as_string_array("expected_edge_ids");
    array<int> expected_weights = jp.as_int_array("expected_weights");
    logger.debug("after remove: ids {} weights {}", got_ids, got_weights);

    bool result = removed && got_ids == expected_ids && got_weights == expected_weights;
    g->cleanup();
    delete g;
    return result;
}

// out-of-range edge access is safe: null vertex, false remove, zero weight
template<typename T>
bool testOutOfRangeEdgeAccess(prb17::utils::parsers::json_parser jp) {
    graph<T>* g = build_weighted_graph_from_config<T>(jp);

    std::string node_id = jp.as_string("node_id");
    size_t oor_idx = jp.as_int("oor_idx");
    vertex<T>* v = g->get(node_id);

    bool result = true;
    result &= v->get_connected_vertex(oor_idx) == nullptr;
    result &= v->remove_edge(oor_idx) == false;
    result &= v->get_weight(oor_idx) == 0;
    logger.debug("out-of-range access safe: '{}'", result);

    g->cleanup();
    delete g;
    return result;
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;

    tests.add(prb17::utils::test{"testNodeCount", &testNodeCount<T>});
    tests.add(prb17::utils::test{"testGetById", &testGetById<T>});
    tests.add(prb17::utils::test{"testGetMissingReturnsNull", &testGetMissingReturnsNull<T>});
    tests.add(prb17::utils::test{"testDuplicateAddRejected", &testDuplicateAddRejected<T>});
    tests.add(prb17::utils::test{"testDfsOrder", &testDfsOrder<T>});
    tests.add(prb17::utils::test{"testBfsOrder", &testBfsOrder<T>});
    tests.add(prb17::utils::test{"testCycleTermination", &testCycleTermination<T>});
    tests.add(prb17::utils::test{"testToStringContainsAll", &testToStringContainsAll<T>});
    tests.add(prb17::utils::test{"testEmptyGraphRenders", &testEmptyGraphRenders<T>});
    tests.add(prb17::utils::test{"testAdjacencyList", &testAdjacencyList<T>});
    tests.add(prb17::utils::test{"testWeightReadback", &testWeightReadback<T>});
    tests.add(prb17::utils::test{"testRemoveEdgeKeepsWeightsAligned", &testRemoveEdgeKeepsWeightsAligned<T>});
    tests.add(prb17::utils::test{"testOutOfRangeEdgeAccess", &testOutOfRangeEdgeAccess<T>});

    return tests;
}

template<typename T>
static prb17::utils::structures::array<prb17::utils::test> graph_tests = build_tests<T>();

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

    validator.add_tests(graph_tests<int>);

    logger.info("Starting validation tests of graph_tests");
    validator.validate();
    logger.info("Finished validation tests of graph_tests");
}
