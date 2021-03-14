#pragma once
#include <chrono>
#include <string>
#include <ctime>
#include <iomanip>

class Stopwatch {
public:
    Stopwatch() {}
    void start() {
        strt = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        stp = std::chrono::high_resolution_clock::now();
    }

    long timestamp() {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(stp - strt).count();
    }

    long timestamp_now() {
        return std::chrono::high_resolution_clock::now().time_since_epoch().count();
    }

    std::string timestamp_now_string() {        
        return timestamp_toString(timestamp_now());
    }

    std::string timestamp_toString(long ts) {
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

private:
    static std::chrono::time_point<std::chrono::high_resolution_clock> strt;
    static std::chrono::time_point<std::chrono::high_resolution_clock> stp;
};