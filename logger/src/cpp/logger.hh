#pragma once
#include <string>
#include <sstream>
#include <ostream>
#include <mutex>
#include <map>
#include <filesystem>
#include <functional>

#include "json_parser.hh"
#include "time.hh"

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
#define TOKEN_IDENTIFIER std::string("#");

#define MONTH std::string("#H")
#define DAY std::string("#A")
#define YEAR std::string("#Y")
#define TIME std::string("#t")
#define MILLIS std::string("#k")
#define MICROS std::string("#u")
#define NANOS std::string("#n")

#define NAME std::string("#N")
#define LEVEL std::string("#L")
#define MSG std::string("#M")

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
				std::string logger_name;
				FILE* logger_file;
				LOG_LEVELS logger_level;
				std::string logger_template;
				std::mutex logger_lock;

				void initialize(std::string);
				std::string tokenReplaceDate(std::string);
				std::string tokenReplaceLevel(std::string, LOG_LEVELS);
				std::string tokenReplaceName(std::string, std::string);
				std::string tokenReplaceMsg(std::string, std::string);

				static void initializeRootLogger();
				static bool root_logger_initialized;
				static LOG_LEVELS root_logger_level;
				static structures::array<std::string> appenders;

				template<typename T>
				static std::map<std::string, std::function<std::string(std::string, std::string, T)> > token_map;
				template<typename T>
				static std::string tokenReplacer(std::string, std::string, T);
				template<typename T, typename... Args>
				static std::string tokenReplacer(std::string, std::string, T, Args...);
				template<typename... Args>
				void log(LOG_LEVELS, std::string, Args...);
				void log(LOG_LEVELS, std::string);
		};

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

		logger::logger(std::string name) {
			initialize(name);
			initializeRootLogger();
		}

		logger::logger(logger const &other) {
			logger_name = other.logger_name;
			logger_file = other.logger_file;
			logger_level = other.logger_level;
		}

		logger& logger::operator=(logger const& other) {
			if (&other != this) {
				logger_name = other.logger_name;
				logger_file = other.logger_file;
				logger_level = other.logger_level;
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
		 * @param name 
		 */
		void logger::initialize(std::string name) {
			parsers::json_parser jp{};
			logger_file = stdout;
			std::string logger_conf_file = std::filesystem::canonical("/proc/self/exe").parent_path().parent_path();
			logger_conf_file = logger_conf_file + "/config/logger_conf.json";
			try {
				jp.parse(logger_conf_file);
				this->logger_name = jp.json_value(name).as_string("logger_name");
				std::string file_name = jp.json_value(name).as_string("logger_file");
				this->logger_file = file_name.empty() ? nullptr : fopen(file_name.c_str(), "a");
				this->logger_level = level_map_str[jp.json_value(name).as_string("logger_level")];
				this->logger_template = jp.as_string("logger_template") + "\n";
			} catch (prb17::utils::exception e) {
				trace("{} not found", logger_conf_file);
				this->logger_name = "";
				this->logger_file = stdout;
				this->logger_level = LOG_LEVELS::INFO;
				this->logger_template = "[#H-#A-#Y #t] [#N] [#L] #M\n";
			}
		}

		bool logger::root_logger_initialized = false;
		structures::array<std::string> logger::appenders{};
		LOG_LEVELS logger::root_logger_level = LOG_LEVELS::ALL;
		void logger::initializeRootLogger() {
			if (!root_logger_initialized) {
				std::string logger_conf_file = std::filesystem::canonical("/proc/self/exe").parent_path().parent_path();
				logger_conf_file = logger_conf_file + "/config/logger_conf.json";
				parsers::json_parser jp{};
				jp.parse(logger_conf_file);
				root_logger_level = level_map_str[jp.json_value("root").as_string("logger_level")];
				appenders = structures::array<std::string>{jp.json_value("root").as_string_array("appenders")};
				root_logger_initialized = true;
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
			log(LOG_LEVELS::TRACE, msg, args...);
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
			log(LOG_LEVELS::DEBUG, msg, args...);
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
			log(LOG_LEVELS::INFO, msg, args...);
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
			log(LOG_LEVELS::WARN, msg, args...);
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
			log(LOG_LEVELS::ERROR, msg, args...);
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
		std::string logger::tokenReplacer(std::string msg, std::string token, T t) {
			std::string::size_type n = msg.find(token);
			if (n != std::string::npos) {
				std::stringstream ss;
				ss << t;
				msg.replace(n, token.length(), ss.str());
			} 
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
		std::string logger::tokenReplacer(std::string msg, std::string token, T t, Args... args) {
			return msg.find(token) == std::string::npos ? msg : tokenReplacer(tokenReplacer(msg, token, t), token, args...);
		}

		/**
		 * @brief 
		 * @todo - This function is probably wasteful when a user configures the logger to not have certain functionality, how to ignore parameters that aren't required by the user?
		 * @param msg_template 
		 * @return std::string 
		 */
		std::string logger::tokenReplaceDate(std::string msg_template) {
			std::string month;
			std::string day;
			std::string year;
			std::string time;
			std::string millis;
			std::string micros;
			std::string nanos;

			time::timestamp_fillin(month, day, year, time, millis, micros, nanos);

			msg_template = tokenReplacer(msg_template, MONTH, month);
			msg_template = tokenReplacer(msg_template, DAY, day);
			msg_template = tokenReplacer(msg_template, YEAR, year);
			msg_template = tokenReplacer(msg_template, TIME, time);
			msg_template = tokenReplacer(msg_template, MILLIS, millis);
			msg_template = tokenReplacer(msg_template, MICROS, micros);
			msg_template = tokenReplacer(msg_template, NANOS, nanos);
			return msg_template;
		}

		/**
		 * @brief 
		 * 
		 * @param msg_template 
		 * @param level 
		 * @return std::string 
		 */
		std::string logger::tokenReplaceLevel(std::string msg_template, LOG_LEVELS level) {
			return tokenReplacer(msg_template, LEVEL, level_map[level]);
		}

		/**
		 * @brief 
		 * 
		 * @param msg_template 
		 * @param name 
		 * @return std::string 
		 */
		std::string logger::tokenReplaceName(std::string msg_template, std::string name) {
			return tokenReplacer(msg_template, NAME, logger_name);
		}

		/**
		 * @brief 
		 * 
		 * @param msg_template 
		 * @param msg 
		 * @return std::string 
		 */
		std::string logger::tokenReplaceMsg(std::string msg_template, std::string msg) {
			return tokenReplacer(msg_template, MSG, msg);
		}

		/**
		 * @brief 
		 * 
		 * @tparam Args 
		 * @param level 
		 * @param msg 
		 * @param args 
		 */
		template<typename... Args>
		void logger::log(LOG_LEVELS level, std::string msg, Args... args) {
			if (level >= logger_level) {
				msg = tokenReplacer(msg, PLACEHOLDER, args...);
				log(level, msg);
			}			
		}

		/**
		 * @brief 
		 * 
		 * @param level 
		 * @param msg 
		 */
		void logger::log(LOG_LEVELS level, std::string msg) {
			if (logger_file && level >= logger_level) {
				if (level >= root_logger_level && appenders.find(logger_name) > -1) {
					std::string tmp;
					tmp = tokenReplaceName(logger_template, logger_name);
					tmp = tokenReplaceDate(tmp);
					tmp = tokenReplaceLevel(tmp, level);				
					msg = tokenReplaceMsg(tmp, msg);
					std::lock_guard<std::mutex> guard(logger_lock);
					fwrite(msg.data(), sizeof(char), msg.length(), logger_file);
				}				
			}
		}
	}
}
