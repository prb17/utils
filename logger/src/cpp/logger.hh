#pragma once
#include <string>
#include <sstream>
#include <ostream>
#include <mutex>
#include <map>
#include <filesystem>

#include "json_parser.hh"

enum class LOG_LEVELS {
	ALL,
	TRACE,
	DEBUG,
	INFO,
	WARN,
	ERROR
};
#define GRY std::string("\033[0;37m")
#define GRE std::string("\033[0;32m")
#define YEL std::string("\033[0;33m")
#define RED std::string("\033[0;31m")
#define NC std::string("\033[0m")

#define PLACEHOLDER std::string("{}")
#define NAME std::string("#n")
#define LEVEL std::string("#l")
#define DATE std::string("#d")
#define MSG std::string("#m")
#define LOG_TEMPLATE std::string("[#d] [#n] [#l] #m\n")

#define TRACE(msg) trace(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + std::string(msg)) 
#define DEBUG(msg) debug(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + std::string(msg)) 
#define INFO(msg) info(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + std::string(msg))
#define WARN(msg) warn(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + std::string(msg))
#define ERROR(msg) error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " + std::string(msg))


static std::map<LOG_LEVELS, std::string> level_map = {
	{LOG_LEVELS::TRACE, "TRACE"},
	{LOG_LEVELS::DEBUG, GRY + "DEBUG" + NC},
	{LOG_LEVELS::INFO, GRE + "INFO" + NC},
	{LOG_LEVELS::WARN, YEL + "WARN" + NC},
	{LOG_LEVELS::ERROR, RED + "ERROR" + NC}
};
static std::map<std::string, LOG_LEVELS> level_map_str = {
	{"trace", LOG_LEVELS::TRACE},
	{"debug", LOG_LEVELS::DEBUG},
	{"info", LOG_LEVELS::INFO},
	{"warn", LOG_LEVELS::WARN},
	{"error", LOG_LEVELS::ERROR}
};
// static std::string logger_conf_file = std::filesystem::canonical("/proc/self/exe").parent_path() + std::string("../config/logger_conf.json");

namespace prb17 {
    namespace utils {		
		class logger {
			public:
				logger(std::string);

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
				FILE* logger_file;
				LOG_LEVELS ignore_level;
				std::string logger_template;
				std::mutex logMutex;

				template<typename T>
				static std::string argsHelper(std::string, std::string, T);
				template<typename T, typename... Args>
				static std::string argsHelper(std::string, std::string, T, Args...);
				void log(LOG_LEVELS, std::string);
		};

		logger::logger(std::string name) {
			parsers::json_parser jp{};
			logger_file = stdout;
			std::string logger_conf_file = std::filesystem::canonical("/proc/self/exe").parent_path();
			logger_conf_file = logger_conf_file + "/../config/logger_conf.json";
			try {
				jp.parse(logger_conf_file);
				this->name = jp.json_value(name).as_string("logger_name");
				this->ignore_level = level_map_str[jp.json_value(name).as_string("ignore_level")];
				this->logger_template = jp.as_string("logger_template");
			} catch (prb17::utils::exception e) {
				warn("Problem parsing {}, using default values", logger_conf_file);
				this->name = "default";
				this->ignore_level = LOG_LEVELS::INFO;
				this->logger_template = "[#n] [#l] #m\n";
			}
            
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
			log(LOG_LEVELS::TRACE, msg);
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
			log(LOG_LEVELS::DEBUG, msg );
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
			log(LOG_LEVELS::INFO, msg);
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
			log(LOG_LEVELS::WARN, msg);
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
			log(LOG_LEVELS::ERROR, msg);
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
			// find first index of {}
			std::string::size_type n = msg.find(token);
			if (n != std::string::npos) {
				// if it exists, replace with string
				std::stringstream ss;
				ss << t;
				msg.replace(n, token.length(), ss.str());
			} //if not, return msg as is
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
			std::string tmp = argsHelper(logger_template, NAME, name);
			tmp = argsHelper(tmp, DATE, "04-03-2022 00:00:00");
			tmp = argsHelper(tmp, LEVEL, level_map[level]);
			msg = argsHelper(tmp, MSG, msg);
			std::lock_guard<std::mutex> guard(logMutex);
			if (logger_file && level >= ignore_level) {
				fwrite(msg.data(), sizeof(char), msg.length(), logger_file);
			}
		}
	}
}
