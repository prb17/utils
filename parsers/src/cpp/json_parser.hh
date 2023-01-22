#pragma once
#include<string>
#include<iostream>

#include "json/json.h"
#include "array.hh"

namespace prb17 {
    namespace utils {
        namespace parsers {
            class json_parser {
                private:
                    Json::Value root;

                public:
                    //constructors
                    json_parser();
                    json_parser(Json::Value&);

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

                    prb17::utils::structures::array<std::string> as_string_array(std::string);
                    prb17::utils::structures::array<int> as_int_array(std::string);
                    prb17::utils::structures::array<uint> as_uint_array(std::string);
                    prb17::utils::structures::array<bool> as_bool_array(std::string);
                    prb17::utils::structures::array<float> as_float_array(std::string);
                    prb17::utils::structures::array<double> as_double_array(std::string);

                    template<typename T>
                    prb17::utils::structures::array<T> as_array(std::string);
            };
            
            inline std::ostream& operator<<(std::ostream &stream, const json_parser &jp) {
                return stream << jp.to_string();
            }

            template<typename T>
            prb17::utils::structures::array<T> json_parser::as_array(std::string property) {
                if (std::is_same<T, std::string>::value) {
                    return as_string_array(property);
                }
                else if (std::is_same<T, int>::value) {
                    return as_int_array(property);
                }
                else if (std::is_same<T, uint>::value) {
                    return as_uint_array(property);
                }
                else if (std::is_same<T, bool>::value) {
                    return as_bool_array(property);
                }
                else if (std::is_same<T, float>::value) {
                    return as_float_array(property);
                }
                else if (std::is_same<T, double>::value) {
                    return as_double_array(property);
                } 
                else {
                    throw exception("unsupported requested data type");
                }
            }
        }
    }
}