#include "structures_director.hh"
#include "graph_builder.hh"
#include "json_parser.hh"

using namespace prb17::utils::structures;

template<typename T>
graph<T>* build_graph_from_config(prb17::utils::parsers::json_parser jp) {
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
