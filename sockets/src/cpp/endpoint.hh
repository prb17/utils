#pragma once

#include <string>
#include <cstdint>

namespace prb17 {
    namespace utils {
        namespace sockets {

            /**
             * @brief An address/port pair identifying one end of a connection.
             *
             * Rendered as "address:port"; from_string parses that form back. A
             * malformed string (no ':') yields an empty endpoint (port 0).
             */
            struct endpoint {
                std::string address;
                uint16_t port;

                endpoint(std::string address = "", uint16_t port = 0)
                    : address{address}, port{port} {}

                std::string to_string() const {
                    return address + ":" + std::to_string(port);
                }

                static endpoint from_string(const std::string& str) {
                    size_t pos = str.find_last_of(':');
                    if (pos == std::string::npos) {
                        return endpoint{};
                    }
                    std::string addr = str.substr(0, pos);
                    std::string port_str = str.substr(pos + 1);
                    uint16_t port = port_str.empty() ? 0 : (uint16_t)std::stoul(port_str);
                    return endpoint{addr, port};
                }

                bool operator==(const endpoint& other) const {
                    return address == other.address && port == other.port;
                }
                bool operator!=(const endpoint& other) const {
                    return !(*this == other);
                }
            };
        }
    }
}
