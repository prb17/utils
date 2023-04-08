#pragma once
#include <chrono>
#include <string>
#include <ctime>
#include <iomanip>

namespace prb17 {
    namespace utils {
        namespace time {
            static long now() {
                return std::chrono::high_resolution_clock::now().time_since_epoch().count();
            }

            static void timestamp_fillin(std::string& mon, std::string& day, std::string& year, std::string& time, std::string& mil, std::string& mic, std::string &nan) {
                long ts = now();

                std::stringstream nss;
                int ns = ts % 1000;
                nss << std::setfill('0') << std::setw(3) << ns;
                nan = nss.str();
                nss.clear();

                std::stringstream uss;
                int us = ((ts % 1000000) - ns)/1000;
                uss << std::setfill('0') << std::setw(3) << us;
                mic = uss.str();
                uss.clear();

                std::stringstream mss;
                int ms = ((ts % 1000000000) - (us+ns))/1000000;
                mss << std::setfill('0') << std::setw(3) << ms;
                mil = mss.str();
                mss.clear();

                std::chrono::time_point<std::chrono::high_resolution_clock> tp((std::chrono::nanoseconds)ts);
                std::time_t t_c = std::chrono::high_resolution_clock::to_time_t(tp);
                
                std::stringstream tss;
                tss << std::put_time(std::localtime(&t_c), "%T");
                time = tss.str();
                tss.clear();

                std::stringstream dss;
                dss << std::put_time(std::localtime(&t_c), "%d");
                day = dss.str();
                dss.clear();

                std::stringstream moss;
                moss << std::put_time(std::localtime(&t_c), "%m");
                mon = moss.str();
                moss.clear();

                std::stringstream yss;
                yss << std::put_time(std::localtime(&t_c), "%Y");
                year = yss.str();
                yss.clear();
            }

            static std::string timestamp_toString(long ts) {
                std::chrono::time_point<std::chrono::high_resolution_clock> tp((std::chrono::nanoseconds)ts);
                std::time_t t_c = std::chrono::high_resolution_clock::to_time_t(tp);        
                std::stringstream ss;
                int ns = ts % 1000;
                int us = ((ts % 1000000) - ns)/1000;
                int ms = ((ts % 1000000000) - (us+ns))/1000000;
                ss << "[" << std::put_time(std::localtime(&t_c), "%D %T") << "." << 
                    std::to_string(ms) << "." << std::to_string(us) << "." << std::to_string(ns) << "] ";
                return ss.str();
            }

            static std::string timestamp_now_string() {        
                return timestamp_toString(now());
            }

            namespace stopwatch {
                static std::chrono::time_point<std::chrono::high_resolution_clock> strt;
                static std::chrono::time_point<std::chrono::high_resolution_clock> stp;

                static void start() {
                    strt = std::chrono::high_resolution_clock::now();
                }

                static void stop() {
                    stp = std::chrono::high_resolution_clock::now();
                }

                static long diff() {
                    return std::chrono::duration_cast<std::chrono::nanoseconds>(stp - strt).count();
                }
            }
        }        
    }
}
