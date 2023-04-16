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

        /**
         * @brief 
         * @todo - could this function logic be better? 3 for loops is very ehh and looks a bit clumsy
         */
        void validator::validate() {
            size_t total_passed = 0;
            size_t total_failed = 0;
            for (int i=0; i<test_files.size(); i++) {
                std::string test_file = test_files[i];
                parsers::json_parser jp{};
                jp.parse(test_file);

                std::string test_name = jp.json_value("MetaData").as_string("test_name");
                logger.debug("Running test file: " + test_file);
                logger.debug(jp.json_value("MetaData").to_string());
                for (auto const &test_to_run : jp.json_value("Tests").get_json_value().getMemberNames()) {
                    size_t current_total_passed = 0;
                    size_t current_total_failed = 0;
                    bool passed = false;
                    logger.debug("###############################");
                    logger.debug("Running test: '{}'", test_to_run);
                    parsers::json_parser tmp{jp.json_value("Tests").json_value(test_to_run)};
                    try {
                        for(Json::Value value : tmp.get_json_value()) {
                            parsers::json_parser test_config{value};
                            auto it = tests_to_validate.find(test_to_run);
                            if (it != tests_to_validate.end()) {
                                passed = it->second(test_config);
                            }                            
                            passed ? current_total_passed++ : current_total_failed++;
                        }
                    } catch( std::exception e) {
                        logger.warn("Exception happend: {}", e.what());
                    }
                    logger.info("'{}' results: {} and {}", test_to_run,
                    GRE + std::to_string(current_total_passed) + " passed" + NC, 
                    RED + std::to_string(current_total_failed) + " failed" + NC
                    );
                    logger.debug("###############################");
                    total_passed += current_total_passed;
                    total_failed += current_total_failed;
                }
            }
            logger.info("Total results: {} and {}", 
            GRE + std::to_string(total_passed) + " passed" + NC, 
            RED + std::to_string(total_failed) + " failed" + NC
            );
        }
    }
}