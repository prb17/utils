#pragma once
#include<iostream>
#include<map>
#include<functional>

#include "json_parser.hh"

namespace prb17 {
    namespace utils {
        class validator {
            private:
                parsers::json_parser jp;
                std::string test_file;
                std::map<std::string, std::function<bool(parsers::json_parser)> > tests_to_validate;

                void print_test_info();

            public:
                validator(std::string);
                void validate();
                void add_tests(std::map<std::string, std::function<bool(parsers::json_parser)> > *);
        };

        validator::validator(std::string file) : jp{}, test_file{file}, tests_to_validate{} {
            jp.parse(test_file);
        }

        void validator::print_test_info() {
            std::cout << "Running test file: " << test_file << std::endl;
            std::cout << jp.json_value("MetaData") << std::endl;
        }

        void validator::add_tests(std::map<std::string, std::function<bool(parsers::json_parser)> > *tests) {
            tests_to_validate.insert(tests->begin(), tests->end());
        }

        void validator::validate() {
            print_test_info();
            for (Json::Value value : jp.json_value("Tests").get_json_value()) {
                std::string test_to_run = value["name"].asString();
                auto it = tests_to_validate.find(test_to_run);
                if (it != tests_to_validate.end()) {
                    std::cout << "###############################" << std::endl;
                    std::cout << "Running test: " << test_to_run << std::endl;
                    parsers::json_parser tmp{value};
                    bool passed = it->second(tmp);
                    std::cout << "Test: " << test_to_run << " result: " << (passed ? "PASSED" : "FAILED") << std::endl;
                    std::cout << "###############################" << std::endl;

                } else {
                    throw exception("config test file provided used unrecognized test name, please fix now");
                }
            }
        }

    }
}