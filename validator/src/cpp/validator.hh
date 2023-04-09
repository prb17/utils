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
                prb17::utils::logger logger;
                prb17::utils::structures::array<std::string> test_files;
                std::map<std::string, std::function<bool(parsers::json_parser)> > tests_to_validate;

            public:
                validator(prb17::utils::structures::array<std::string>);
                void validate();
                void add_tests(std::map<std::string, std::function<bool(parsers::json_parser)> > *);
        };

        validator::validator(prb17::utils::structures::array<std::string> files) : logger{"validator"}, test_files{files}, tests_to_validate{} {}

        void validator::add_tests(std::map<std::string, std::function<bool(parsers::json_parser)> > *tests) {
            tests_to_validate.insert(tests->begin(), tests->end());
        }

        void validator::validate() {
            size_t total_passed = 0;
            size_t total_failed = 0;
            for (int i=0; i<test_files.size(); i++) {
                std::string test_file = test_files[i].value();
                parsers::json_parser jp{};
                jp.parse(test_file);

                size_t current_total_passed = 0;
                size_t current_total_failed = 0;
                std::string test_name = jp.json_value("MetaData").as_string("test_name");
                logger.debug("Running test file: " + test_file);
                logger.debug(jp.json_value("MetaData").to_string());
                for (Json::Value value : jp.json_value("Tests").get_json_value()) {
                    std::string test_to_run = value["name"].asString();
                    auto it = tests_to_validate.find(test_to_run);
                    if (it != tests_to_validate.end()) {
                        logger.debug("###############################");
                        logger.debug("Running test: " + test_to_run);
                        parsers::json_parser tmp{value};
                        bool passed = it->second(tmp);
                        logger.debug("Test: " + test_to_run + " result: " + (passed ? GRE + "PASSED" + NC : RED + "FAILED" + NC));
                        passed ? current_total_passed++ : current_total_failed++;
                        logger.debug("###############################");

                    } else {
                        throw exception("config test file provided used unrecognized test name, please fix now");
                    }
                }
                logger.debug("'{}' results: {} and {}", test_name,
                GRE + std::to_string(current_total_passed) + " passed" + NC, 
                RED + std::to_string(current_total_failed) + " failed" + NC
                );
                total_passed += current_total_passed;
                total_failed += current_total_failed;
            }
            logger.info("Total results: {} and {}", 
            GRE + std::to_string(total_passed) + " passed" + NC, 
            RED + std::to_string(total_failed) + " failed" + NC
            );
        }
    }
}