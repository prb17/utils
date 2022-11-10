#pragma once

#include <exception>

namespace pbrady {
    namespace utils {
        class exception : public std::exception {
        private:
            std::string message;
        public:
            exception() throw();
            exception(std::string) throw();
            virtual char const* what() const throw();
        };

        exception::exception() throw()
            : exception("Utility exception ocurred.") {}

        exception::exception(std::string msg) throw()
            : std::exception(), message{msg} {}

        char const * exception::what() const throw() {
            return message.c_str();
        }
    }
}
