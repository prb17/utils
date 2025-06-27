#include "map.hh"
#include "validator.hh"
#include "logger.hh"

#include <iostream>
#include <memory>
#include <map>

using namespace prb17::utils::structures;
static prb17::utils::logger logger{"map_test"};

template<typename V>
bool basic_map_print(prb17::utils::parsers::json_parser jp) {
    logger.info("Building map");
    map m = map<std::string, V>{};

    for(Json::Value::ArrayIndex i=0; i < jp.get_json_value()["map"]["pairs"].size(); i++) {
        prb17::utils::parsers::json_parser tmp{jp.get_json_value()["map"]["pairs"][i]};
        logger.info("Adding element '{}' to map", i);
        std::string key = tmp.as_value<std::string>("key");
        V value = tmp.as_value<V>("value");
        logger.info("adding key: '{}', and value: '{}'", key, value);
        m.add(key, value);
    }
    logger.info("The map to_string(): {}", m);
    return true;
}

template<typename V>
static prb17::utils::structures::array<prb17::utils::test> build_tests() {
    prb17::utils::structures::array<prb17::utils::test> tests;

    tests.add(prb17::utils::test{"basicMapPrint", &basic_map_print<V>});

    return tests;
}
template<typename V>
static prb17::utils::structures::array<prb17::utils::test> map_tests = build_tests<V>();

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
    
    validator.add_tests(map_tests<std::string>);
    validator.add_tests(map_tests<int>);
    validator.add_tests(map_tests<uint>);
    validator.add_tests(map_tests<char>);
    validator.add_tests(map_tests<bool>);
    validator.add_tests(map_tests<float>);
    validator.add_tests(map_tests<double>);

    logger.info("Starting validation tests of map_tests");
    validator.validate();
    logger.info("Finished validation tests of map_tests");
}
