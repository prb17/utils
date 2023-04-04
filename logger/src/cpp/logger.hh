#pragma once
#include <string>
#include <sstream>
#include <ostream>
#include <mutex>

enum class LOG_LEVELS {
	ALL,
	TRACE,
	DEBUG,
	INFO,
	WARN,
	ERROR
};
#define GRY "\033[0;37m"
#define GRE "\033[0;32m"
#define YEL "\033[0;33m"
#define RED "\033[0;31m"
#define NC "\033[0m"

#define PLACEHOLDER std::string("{}")
#define NAME std::string("#n")
#define LEVEL std::string("#l")
#define DATE std::string("#d")
#define MSG std::string("#m")
#define LOG_TEMPLATE std::string("[#d] [#n] [#m] [#l]\n")

#define TRACE(msg) trace(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + std::string(msg)) 
#define DEBUG(msg) debug(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + std::string(msg)) 
#define INFO(msg) info(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + std::string(msg))
#define WARN(msg) warn(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + std::string(msg))
#define ERROR(msg) error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + std::string(msg))

namespace prb17 {
    namespace utils {		
		class logger {
			public:

				logger(std::string);
				logger(std::string, LOG_LEVELS);
				logger(std::string, LOG_LEVELS, std::string);

				logger(logger const&);
				logger& operator=(logger const&);
				~logger();

				template<typename... Args>
				void trace(std::string, Args...);
				void trace(std::string);

				template<typename... Args>
				void debug(std::string, Args...);
				void debug(std::string);
				
				template<typename... Args>
				void info(std::string, Args...);
				void info(std::string);
				
				template<typename... Args>
				void warn(std::string, Args...);
				void warn(std::string);
				
				template<typename... Args>
				void error(std::string, Args...);
				void error(std::string);
			private:
				std::string name;
				std::string pre_msg;
				FILE* logger_file;
				LOG_LEVELS ignore_level;
				std::mutex logMutex;

				template<typename T>
				static std::string argsHelper(std::string, std::string, T);
				template<typename T, typename... Args>
				static std::string argsHelper(std::string, std::string, T, Args...);
				void log(LOG_LEVELS, std::string);
		};

		logger::logger(std::string name) : logger(name, LOG_LEVELS::ALL) {}
		logger::logger(std::string name, LOG_LEVELS level) : logger(name, level, "") {}
		logger::logger(std::string name,  LOG_LEVELS level, std::string file_name) : name{name}, ignore_level{level}, logger_file{file_name.empty() ? stdout : fopen(file_name.c_str(), "a")} {
			pre_msg = argsHelper(LOG_TEMPLATE, NAME, name);
			pre_msg = argsHelper(pre_msg, LEVEL, "LOG LEVEL");
			pre_msg = argsHelper(pre_msg, DATE, "04-03-2022");
		}

		logger::logger(logger const &other) {
			name = other.name;
			logger_file = other.logger_file;
			ignore_level = other.ignore_level;
		}

		logger& logger::operator=(logger const& other) {
			if (&other != this) {
				name = other.name;
				logger_file = other.logger_file;
				ignore_level = other.ignore_level;
			}
			return *this;
		}

		logger::~logger() {
			if(logger_file) {
				fclose(logger_file);
			}	
		}
		

		/**
		 * @brief 
		 * 
		 * @tparam Args 
		 * @param msg 
		 * @param args 
		 */
		template<typename... Args>
		void logger::trace(std::string msg, Args... args) {
			trace(argsHelper(msg, PLACEHOLDER, args...));
		}
		void logger::trace(std::string msg) {
			log(LOG_LEVELS::TRACE, std::string("[TRACE] ") + msg);
		}

		/**
		 * @brief 
		 * 
		 * @tparam Args 
		 * @param msg 
		 * @param args 
		 */
		template<typename... Args>
		void logger::debug(std::string msg, Args... args) {
			debug(argsHelper(msg, PLACEHOLDER, args...));
		}
		void logger::debug(std::string msg) {
			log(LOG_LEVELS::DEBUG, "[" + std::string(GRY) + "DEBUG" + std::string(NC) + "]" + " " + msg );
		}

		/**
		 * @brief 
		 * 
		 * @tparam Args 
		 * @param msg 
		 * @param args 
		 */
		template<typename... Args>
		void logger::info(std::string msg, Args... args) {
			info(argsHelper(msg, PLACEHOLDER, args...));
		}
		void logger::info(std::string msg) {
			log(LOG_LEVELS::INFO, "[" + std::string(GRE) + "INFO" + std::string(NC) + "]" + " " + msg);
		}

		/**
		 * @brief 
		 * 
		 * @tparam Args 
		 * @param msg 
		 * @param args 
		 */
		template<typename... Args>
		void logger::warn(std::string msg, Args... args) {
			warn(argsHelper(msg, PLACEHOLDER, args...));
		}
		void logger::warn(std::string msg) {
			log(LOG_LEVELS::WARN, "[" + std::string(YEL) + "WARN" + std::string(NC) + "]" + " " + msg);
		}

		/**
		 * @brief 
		 * 
		 * @tparam Args 
		 * @param msg 
		 * @param args 
		 */
		template<typename... Args>
		void logger::error(std::string msg, Args... args) {
			error(argsHelper(msg, PLACEHOLDER, args...));
		}
		void logger::error(std::string msg) {
			log(LOG_LEVELS::ERROR, "[" + std::string(RED) + "ERROR" + std::string(NC) + "]" + " " + msg);
		}


		/**
		 * @brief 
		 * 
		 * @tparam T 
		 * @param msg 
		 * @param t 
		 * @return std::string 
		 */
		template<typename T>
		std::string logger::argsHelper(std::string msg, std::string token, T t) {
			// std::cout << __PRETTY_FUNCTION__ << std::endl ;
			//find first index of {}
			std::string::size_type n = msg.find(token);
			if (n != std::string::npos) {
				//if it exists, replace with string
				std::stringstream ss;
				ss << t;
				msg.replace(n, token.length(), ss.str());
			}//if not, return msg as is
			return msg;
		}
		/**
		 * @brief 
		 * 
		 * @tparam T 
		 * @tparam Args 
		 * @param msg 
		 * @param t 
		 * @param args 
		 * @return std::string 
		 */
		template<typename T, typename... Args>
		std::string logger::argsHelper(std::string msg, std::string token, T t, Args... args) {
			return msg.find(token) == std::string::npos ? msg : argsHelper(argsHelper(msg, token, t), token, args...);
		}

		/**
		 * @brief 
		 * 
		 * @param level 
		 * @param msg 
		 */
		void logger::log(LOG_LEVELS level, std::string msg) {
			std::lock_guard<std::mutex> guard(logMutex);
			if (logger_file && level > ignore_level) {
				msg = argsHelper(pre_msg, MSG, msg);
				fwrite(msg.data(), sizeof(char), msg.length(), logger_file);
			}
		}
	}
}
