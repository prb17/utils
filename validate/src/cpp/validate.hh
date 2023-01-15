#pragma once
#include<iostream>
#include "json_parser.hh"

namespace prb17 {
    namespace utils {
        class validate {
            private:
                parsers::json_parser jp;

            public:
                void print_test_info();
                
        };

        void validate::print_test_info() {
            std::cout << jp << std::endl;
        }
    }
}