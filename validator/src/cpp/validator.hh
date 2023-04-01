#pragma once
#include<iostream>
#include<map>
#include<functional>

#include "json_parser.hh"
#include "array.hh"
#include "logger.hh"

namespace prb17 {
    namespace utils {
        class validator {
            private:
                prb17::utils::SimpleLogger logger;
                prb17::utils::structures::array<std::string> test_files;
                std::map<std::string, std::function<bool(parsers::json_parser)> > tests_to_validate;

            public:
                validator(prb17::utils::structures::array<std::string>);
                void validate();
                void add_tests(std::map<std::string, std::function<bool(parsers::json_parser)> > *);
        };

        validator::validator(prb17::utils::structures::array<std::string> files) : logger{}, test_files{files}, tests_to_validate{} {}

        void validator::add_tests(std::map<std::string, std::function<bool(parsers::json_parser)> > *tests) {
            tests_to_validate.insert(tests->begin(), tests->end());
        }

        void validator::validate() {
            for (int i=0; i<test_files.size(); i++) {
                std::string test_file = test_files[i].value();
                parsers::json_parser jp{};
                jp.parse(test_file);
                logger.info("Running test file: " + test_file);
                logger.info(jp.json_value("MetaData").to_string());
                for (Json::Value value : jp.json_value("Tests").get_json_value()) {
                    std::string test_to_run = value["name"].asString();
                    auto it = tests_to_validate.find(test_to_run);
                    if (it != tests_to_validate.end()) {
                        logger.info("###############################");
                        logger.info("Running test: " + test_to_run);
                        parsers::json_parser tmp{value};
                        bool passed = it->second(tmp);
                        logger.info("Test: " + test_to_run + " result: " + (passed ? "PASSED" : "FAILED"));
                        logger.info("###############################");

                    } else {
                        throw exception("config test file provided used unrecognized test name, please fix now");
                    }
                }
            }
            
        }

    }
}