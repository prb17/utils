#pragma once
#include<iostream>
//#include<map>
#include<functional>

#include "json_parser.hh"
#include "array.hh"
#include "logger.hh"
#include "map.hh"

namespace prb17 {
    namespace utils {
        class test {
            private:
                std::string name;
                std::string description;
                std::function<bool(parsers::json_parser)> t;

            public:
                explicit test(std::string n, std::function<bool(parsers::json_parser)> t, std::string d="" ) : name{n}, description{d}, t{t} {}
                std::string get_name() { return name; }
                std::string get_desc() { return description; }

                bool operator() (parsers::json_parser jp) const {
                    return t(jp);
                }

                std::string to_string() const {
                    std::stringstream stream;
                    stream << "{ name: '" << name  << "', desc: '" << description << "' }";
                    return stream.str();
                }
        };

        inline std::ostream& operator<<(std::ostream &stream, const test& t) {
            return stream << t.to_string();
        }
        inline std::ostream& operator<<(std::ostream &stream, const test* t) {
            return stream << t->to_string();
        }

        class validator {
            private:
                prb17::utils::logger logger;
                prb17::utils::structures::array<std::string> test_files;
                prb17::utils::structures::array<test> tests_to_validate;

            public:
                validator(prb17::utils::structures::array<std::string>);
                void validate();
                void add_test(test);
                void add_test(std::string, std::function<bool(parsers::json_parser)>, std::string);
                void add_tests(prb17::utils::structures::array<test>);
        };

        validator::validator(prb17::utils::structures::array<std::string> files) : logger{"validator"}, test_files{files}, tests_to_validate{} {}
        void validator::add_test(test t) {
            tests_to_validate.add( t );
        }
        void validator::add_test(std::string name, std::function<bool(parsers::json_parser)> func, std::string desc) {
            tests_to_validate.add( test{name, func, desc} );
        }
        void validator::add_tests(prb17::utils::structures::array<test> tests) {
            for (int i=0; i<tests.size(); i++) {
                tests_to_validate.add( tests[i] );
            }
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
                    test* func = nullptr;
                    for(int i=0; i<tests_to_validate.size(); i++) {
                        if (tests_to_validate[i].get_name() == test_to_run) {
                            func = &tests_to_validate[i];
                        }
                    }
                    if (func == nullptr) {
                        continue;
                    }
                    size_t current_total_passed = 0;
                    size_t current_total_failed = 0;
                    bool passed = false;
                    logger.debug("###############################");
                    logger.debug("Running test: '{}'", test_to_run);
                    parsers::json_parser tmp{jp.json_value("Tests").json_value(test_to_run)};
                    try {
                        for(Json::Value value : tmp.get_json_value()) {
                            parsers::json_parser test_config{value};
                            //if (it != tests_to_validate.end()) {
                                passed = (*func)(test_config);
                                passed ? current_total_passed++ : current_total_failed++;
                            //}                   
                        }
                    } catch( std::exception e) {
                        logger.warn("Exception happend: {}", e.what());
                        current_total_failed++;
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
