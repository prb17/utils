#pragma once
#include <string>
#include <mutex>

#define GRY "\033[0;37m"
#define GRE "\033[0;32m"
#define YEL "\033[0;33m"
#define RED "\033[0;31m"
#define NC "\033[0m"

#define debug(msg) log(LOG_LEVELS::DEBUG, std::string("[\033[0;37mDEBUG\033[0m] ") + std::string(msg)) 
#define info(msg) log(LOG_LEVELS::INFO, std::string("[\033[0;32mINFO\033[0m] ") + std::string(msg))
#define warn(msg) log(LOG_LEVELS::WARN, std::string("[\033[0;33mWARN\033[0m] ") + std::string(msg))
#define error(msg) log(LOG_LEVELS::ERROR, std::string("[\033[0;31mERROR\033[0m] ") + std::string(msg))

namespace prb17 {
    namespace utils {		
		enum class LOG_LEVELS {
			ALL,
			INFO,
			DEBUG,
			WARN,
			ERROR
		};

		class SimpleLogger {
			public:	
				SimpleLogger();
				SimpleLogger(std::string);

				SimpleLogger(SimpleLogger const&);
				SimpleLogger& operator=(SimpleLogger const&);
				~SimpleLogger();

				void log(LOG_LEVELS, std::string);
				void set_ignore_level(LOG_LEVELS);
			private:
				FILE* logger_file;
				LOG_LEVELS ignore_level;
				std::mutex logMutex;
		};

		SimpleLogger::SimpleLogger()
		{
			logger_file = stdout;
			ignore_level = LOG_LEVELS::ALL;
		}

		SimpleLogger::SimpleLogger(std::string file_name)
		{
			logger_file = fopen(file_name.c_str(), "a");
			ignore_level = LOG_LEVELS::ALL;
		}

		SimpleLogger::SimpleLogger(SimpleLogger const &other) {
			logger_file = other.logger_file;
			ignore_level = other.ignore_level;
		}

		SimpleLogger& SimpleLogger::operator=(SimpleLogger const& other)
		{
			if (&other != this) {

				logger_file = other.logger_file;
				ignore_level = other.ignore_level;
			}
			return *this;
		}

		SimpleLogger::~SimpleLogger() {
			if(logger_file) {
				fclose(logger_file);
			}	
		}

		void SimpleLogger::set_ignore_level(LOG_LEVELS lvl) {
			ignore_level = lvl;
		}

		void SimpleLogger::log(LOG_LEVELS level, std::string msg) {
			std::lock_guard<std::mutex> guard(logMutex);
			if (logger_file && level > ignore_level)
			{
				msg = msg + "\n";
				fwrite(msg.data(), sizeof(char), msg.length(), logger_file);
			}
		}
	}
}
