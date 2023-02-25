#pragma once
#include<string>
#include<iostream>
#include<memory>

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
                    json_parser(const Json::Value&);

                    //modifiers
                    void parse(std::string);

                    //accessors
                    Json::Value get_json_value();
                    void set_json_value(Json::Value);
                    std::string to_string() const;

                    json_parser json_value(std::string);

                    template<typename T>
                    T as_value(std::string);

                    std::string as_string(std::string);
                    int as_int(std::string);
                    uint as_uint(std::string);
                    char as_char(std::string);
                    bool as_bool(std::string);
                    float as_float(std::string);
                    double as_double(std::string);
                    
                    template<typename T>
                    prb17::utils::structures::array<T> as_array(std::string);

                    prb17::utils::structures::array<std::string> as_string_array(std::string);
                    prb17::utils::structures::array<int> as_int_array(std::string);
                    prb17::utils::structures::array<uint> as_uint_array(std::string);
                    prb17::utils::structures::array<char> as_char_array(std::string);
                    prb17::utils::structures::array<bool> as_bool_array(std::string);
                    prb17::utils::structures::array<float> as_float_array(std::string);
                    prb17::utils::structures::array<double> as_double_array(std::string);
            };
            
            inline std::ostream& operator<<(std::ostream &stream, const json_parser &jp) {
                return stream << jp.to_string();
            }

            template<typename T>
            T json_parser::as_value(std::string property) {
                void *val;
                if (std::is_same<T, std::string>::value) {
                    val = new std::string{as_string(property)};
                }
                else if (std::is_same<T, int>::value) {
                    val = new int{as_int(property)};
                }
                else if (std::is_same<T, uint>::value) {
                    val = new uint{as_uint(property)};
                }
                else if (std::is_same<T, char>::value) {
                    val = new char{as_char(property)};
                }
                else if (std::is_same<T, bool>::value) {
                    val = new bool{as_bool(property)};
                }
                else if (std::is_same<T, float>::value) {
                    val = new float{as_float(property)};
                }
                else if (std::is_same<T, double>::value) {
                    val = new double{as_double(property)};
                } 
                else {
                    throw exception("unsupported requested data type");
                }
                T value = *(T*)val;
                delete (T*)val;
                return value;
            }

            template<typename T>
            prb17::utils::structures::array<T> json_parser::as_array(std::string property) {
                void *arr;
                if (std::is_same<T, std::string>::value) {
                    arr = new prb17::utils::structures::array<std::string>{as_string_array(property)};
                }
                else if (std::is_same<T, int>::value) {
                    arr = new prb17::utils::structures::array<int>{as_int_array(property)};
                }
                else if (std::is_same<T, uint>::value) {
                    arr = new prb17::utils::structures::array<uint>{as_uint_array(property)};
                }
                else if (std::is_same<T, char>::value) {
                    arr = new prb17::utils::structures::array<char>{as_char_array(property)};
                }
                else if (std::is_same<T, bool>::value) {
                    arr = new prb17::utils::structures::array<bool>{as_bool_array(property)};
                }
                else if (std::is_same<T, float>::value) {
                    arr = new prb17::utils::structures::array<float>{as_float_array(property)};
                }
                else if (std::is_same<T, double>::value) {
                    arr = new prb17::utils::structures::array<double>{as_double_array(property)};
                } 
                else {
                    throw exception("unsupported requested data type");
                }
                prb17::utils::structures::array<T> array{*(prb17::utils::structures::array<T>*)arr};
                delete (prb17::utils::structures::array<T>*)arr;
                return array;
            }
        }
    }
}