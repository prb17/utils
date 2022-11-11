#pragma once
#include<string>

#include "json/json.h"

namespace prb17 {
    namespace utils {
        namespace parsers {
            class json_parser {
                private:
                    Json::Value root;

                public:
                    //constructors
                    json_parser();

                    //modifiers
                    void parse(std::string);

                    //accessors
                    Json::Value json_value(std::string);
                    std::string as_string(std::string);
                    int as_int(std::string);
            };
        }
    }
}