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
                    Json::Value get_json_value();
                    void set_json_value(Json::Value);
                    std::string to_string() const;

                    json_parser json_value(std::string);
                    std::string as_string(std::string);
                    int as_int(std::string);
                    uint as_uint(std::string);
                    bool as_bool(std::string);
                    float as_float(std::string);
                    double as_double(std::string);
            };
            
            inline std::ostream& operator<<(std::ostream &stream, const json_parser &jp) {
                return stream << jp.to_string();
            }
        }
    }
}