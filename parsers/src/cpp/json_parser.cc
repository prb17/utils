#include<fstream>

#include "json_parser.hh"
#include "exception.hh"

using namespace prb17::utils::parsers;

json_parser::json_parser() {}

void json_parser::parse(std::string filename) {
    std::ifstream fstream(filename);
    Json::Reader reader{};
    
    if (!reader.parse(fstream, root)) {
        throw exception(
            "Can't parse json file: '" + 
            filename + 
            "'. Reason: '" + 
            reader.getFormattedErrorMessages() + 
            "'."
        );
    }
}

Json::Value json_parser::json_value(std::string property) {
    Json::Value retval;
    try {
        retval = root[property];
    } catch (Json::Exception &e) {
        throw utils::exception(e.what());
    }    
    
    return retval;
}

std::string json_parser::as_string(std::string property) {
    std::string retval;
    try {
        retval = root[property].asString();
    } catch (Json::Exception &e) {
        throw utils::exception(e.what());
    }    
    
    return retval;
}