#include "json_parser.hh"

#include <iostream>

using namespace prb17::utils::parsers;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << std::string(argv[0]) << " must have 1 test json file as an argument" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string filename = std::string(argv[1]);
    json_parser parser{};
    parser.parse(filename);

    std::cout << parser.as_string("jsonValue1") << std::endl;
}