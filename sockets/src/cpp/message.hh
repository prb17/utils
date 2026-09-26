#pragma once

#include <string>

#include "endpoint.hh"

namespace prb17 {
    namespace utils {
        namespace sockets {

            /**
             * @brief A structured message passed between endpoints.
             *
             * Serializes to a single line so it can be framed over a socket with a
             * newline terminator: "source|destination|type|body". The first three
             * fields never contain the '|' delimiter or a newline; body is the
             * remainder of the line (may contain '|', must not contain a newline).
             */
            class message {
                private:
                    static const char DELIM = '|';

                public:
                    endpoint source;
                    endpoint destination;
                    std::string type;
                    std::string body;

                    message() : source{}, destination{}, type{}, body{} {}
                    message(endpoint src, endpoint dst, std::string type = "", std::string body = "")
                        : source{src}, destination{dst}, type{type}, body{body} {}

                    std::string to_string() const {
                        return source.to_string() + DELIM
                             + destination.to_string() + DELIM
                             + type + DELIM
                             + body;
                    }

                    static message from_string(const std::string& str) {
                        size_t p1 = str.find(DELIM);
                        size_t p2 = (p1 == std::string::npos) ? std::string::npos : str.find(DELIM, p1 + 1);
                        size_t p3 = (p2 == std::string::npos) ? std::string::npos : str.find(DELIM, p2 + 1);
                        if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos) {
                            return message{};
                        }
                        message m;
                        m.source = endpoint::from_string(str.substr(0, p1));
                        m.destination = endpoint::from_string(str.substr(p1 + 1, p2 - p1 - 1));
                        m.type = str.substr(p2 + 1, p3 - p2 - 1);
                        m.body = str.substr(p3 + 1);
                        return m;
                    }

                    bool operator==(const message& o) const {
                        return source == o.source && destination == o.destination
                            && type == o.type && body == o.body;
                    }
                    bool operator!=(const message& o) const { return !(*this == o); }
            };
        }
    }
}
