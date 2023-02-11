#include "json_parser.hh"
#include "array.hh"
#include <iostream>

using namespace prb17::utils::parsers;

int main() {
    char *f = std::getenv("TEST_CONFIG_FILE");

    if (f == nullptr) {
        std::cout << "must set 'TEST_CONFIG_FILE' env variable to location of config file" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string filename{f};
    json_parser parser{};
    parser.parse(filename);

    std::cout << parser.as_string("jsonValue1") << std::endl;

    std::cout << parser.as_string("jsonIntObject") << std::endl;
    std::cout << parser.as_int("jsonIntObject") << std::endl;

    std::cout << parser.json_value("jsonObject1") << std::endl;
    
    std::cout << parser << std::endl;
    
    std::cout << 
        parser
        .json_value("jsonObject1")
        .as_string("jsonValue3")
    << std::endl;

    std::cout << 
        parser
        .json_value("jsonObject2")
        .json_value("jsonObject3")
        .as_string("jsonValue3")
    << std::endl;

    // std::cout << parser.as_string_array("jsonStringArray") << std::endl;
    std::cout << parser.as_int_array("jsonIntArray") << std::endl;
    std::cout << parser.as_uint_array("jsonUIntArray") << std::endl;
    std::cout << parser.as_bool_array("jsonBoolArray") << std::endl;
    std::cout << parser.as_float_array("jsonFloatArray") << std::endl;
    std::cout << parser.as_double_array("jsonDoubleArray") << std::endl;

    // std::cout << *(prb17::utils::structures::array<std::string>*)parser.as_array<std::string>("jsonStringArray") << std::endl;
    std::cout << *(prb17::utils::structures::array<int>*)parser.as_array<int>("jsonIntArray") << std::endl;
    std::cout << *(prb17::utils::structures::array<uint>*)parser.as_array<uint>("jsonUIntArray") << std::endl;
    std::cout << *(prb17::utils::structures::array<bool>*)parser.as_array<bool>("jsonBoolArray") << std::endl;
    std::cout << *(prb17::utils::structures::array<float>*)parser.as_array<float>("jsonFloatArray") << std::endl;
    std::cout << *(prb17::utils::structures::array<double>*)parser.as_array<double>("jsonDoubleArray") << std::endl;
}