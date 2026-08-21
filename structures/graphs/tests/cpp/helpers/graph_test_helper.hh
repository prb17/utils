#pragma once

#include "structures_director.hh"
#include "graph_builder.hh"
#include "json_parser.hh"

using namespace prb17::utils::structures;

template<typename T>
graph<T>* build_graph_from_config(prb17::utils::parsers::json_parser jp) {
    structures_director d{};
    graph_builder<T> b{};

    // Construct the nodes list. Edges are plain id strings here.
    for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
        prb17::utils::parsers::json_parser tmp{jp.get_json_value()["graph"]["nodes"][i]};
        b.add(tmp.as_string("id"), tmp.as_value<T>("value"), tmp.as_string_array("edges"));
    }

    d.construct(&b);
    return b.graph_product();
}

template<typename T>
graph<T>* build_weighted_graph_from_config(prb17::utils::parsers::json_parser jp) {
    structures_director d{};
    graph_builder<T> b{};

    // Construct the nodes list. Edges are {"id": ..., "weight": ...} objects here.
    for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["graph"]["nodes"].size(); i++) {
        prb17::utils::parsers::json_parser tmp{jp.get_json_value()["graph"]["nodes"][i]};
        prb17::utils::structures::array<std::string> edge_ids{};
        prb17::utils::structures::array<int> edge_weights{};
        for(Json::Value::ArrayIndex j=0; j < jp.get_json_value()["graph"]["nodes"][i]["edges"].size(); j++) {
            std::string edge_id = jp.get_json_value()["graph"]["nodes"][i]["edges"][j]["id"].asString();
            int weight = jp.get_json_value()["graph"]["nodes"][i]["edges"][j]["weight"].asInt();
            edge_ids.add(edge_id);
            edge_weights.add(weight);
        }
        b.add(tmp.as_string("id"), tmp.as_value<T>("value"), edge_ids, edge_weights);
    }

    d.construct(&b);
    return b.graph_product();
}
