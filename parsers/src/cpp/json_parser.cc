#include<fstream>
#include<sstream>

#include "json_parser.hh"
#include "exception.hh"

using namespace prb17::utils::parsers;

json_parser::json_parser() {}

Json::Value json_parser::get_json_value() {
    return root;
}

void json_parser::set_json_value(Json::Value v) {
    root = v;
}

std::string json_parser::to_string() const {
    std::stringstream ss;
    ss << root;
    return ss.str();
}

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

json_parser json_parser::json_value(std::string property) {
    json_parser retval;
    try {
        retval.set_json_value(root[property]);
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

int json_parser::as_int(std::string property) {
    int retval;
    try {
        retval = root[property].asInt();
    } catch (Json::Exception &e) {
        throw utils::exception(e.what());
    }    
    
    return retval;
}

uint json_parser::as_uint(std::string property) {
    uint retval;
    try {
        retval = root[property].asUInt();
    } catch (Json::Exception &e) {
        throw utils::exception(e.what());
    }    
    
    return retval;
}

bool json_parser::as_bool(std::string property) {
    bool retval;
    try {
        retval = root[property].asBool();
    } catch (Json::Exception &e) {
        throw utils::exception(e.what());
    }    
    
    return retval;
}

float json_parser::as_float(std::string property) {
    float retval;
    try {
        retval = root[property].asFloat();
    } catch (Json::Exception &e) {
        throw utils::exception(e.what());
    }    
    
    return retval;
}

double json_parser::as_double(std::string property) {
    double retval;
    try {
        retval = root[property].asDouble();
    } catch (Json::Exception &e) {
        throw utils::exception(e.what());
    }    
    
    return retval;
}
