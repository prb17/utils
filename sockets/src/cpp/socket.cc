#include "socket.hh"

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <cstring>
#include <string>

namespace prb17 {
    namespace utils {
        namespace sockets {

            //=====================================================================
            // socket
            //=====================================================================
            socket::socket() : fd_{-1} {
                fd_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            }

            socket::socket(int fd) : fd_{fd} {}

            socket::socket(socket&& other) noexcept : fd_{other.fd_} {
                other.fd_ = -1;
            }

            socket& socket::operator=(socket&& other) noexcept {
                if (this != &other) {
                    close();
                    fd_ = other.fd_;
                    other.fd_ = -1;
                }
                return *this;
            }

            socket::~socket() {
                close();
            }

            bool socket::valid() const { return fd_ >= 0; }
            int socket::fd() const { return fd_; }

            void socket::close() {
                if (fd_ >= 0) {
                    ::close(fd_);
                    fd_ = -1;
                }
            }

            bool socket::shutdown() {
                if (fd_ < 0) { return false; }
                return ::shutdown(fd_, SHUT_RDWR) == 0;
            }

            bool socket::send_bytes(const char* buffer, size_t n) {
                if (fd_ < 0) { return false; }
                size_t sent = 0;
                while (sent < n) {
                    ssize_t r = ::send(fd_, buffer + sent, n - sent, 0);
                    if (r <= 0) { return false; }
                    sent += (size_t)r;
                }
                return true;
            }

            bool socket::recv_bytes(char* buffer, size_t n) {
                if (fd_ < 0) { return false; }
                size_t got = 0;
                while (got < n) {
                    ssize_t r = ::recv(fd_, buffer + got, n - got, 0);
                    if (r <= 0) { return false; }
                    got += (size_t)r;
                }
                return true;
            }

            bool socket::send_string(const std::string& str, char terminator) {
                std::string framed = str;
                framed.push_back(terminator);
                return send_bytes(framed.data(), framed.size());
            }

            std::string socket::recv_string(char terminator) {
                std::string out;
                if (fd_ < 0) { return out; }
                char c;
                while (true) {
                    ssize_t r = ::recv(fd_, &c, 1, 0);
                    if (r <= 0) { break; }      // peer closed or error
                    if (c == terminator) { break; }
                    out.push_back(c);
                }
                return out;
            }

            //=====================================================================
            // socket_connecter
            //=====================================================================
            socket_connecter::socket_connecter() : socket() {}

            bool socket_connecter::connect(const std::string& host, uint16_t port) {
                struct addrinfo hints;
                std::memset(&hints, 0, sizeof(hints));
                hints.ai_family = AF_UNSPEC;
                hints.ai_socktype = SOCK_STREAM;
                hints.ai_protocol = IPPROTO_TCP;

                struct addrinfo* result = nullptr;
                std::string port_str = std::to_string(port);
                if (::getaddrinfo(host.c_str(), port_str.c_str(), &hints, &result) != 0) {
                    return false;
                }

                bool connected = false;
                for (struct addrinfo* p = result; p != nullptr; p = p->ai_next) {
                    int s = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol);
                    if (s < 0) { continue; }
                    if (::connect(s, p->ai_addr, p->ai_addrlen) == 0) {
                        close();       // release the descriptor from the base ctor
                        fd_ = s;
                        connected = true;
                        break;
                    }
                    ::close(s);
                }
                ::freeaddrinfo(result);
                return connected;
            }

            //=====================================================================
            // socket_listener
            //=====================================================================
            socket_listener::socket_listener(uint16_t port)
                : socket(), port_{port}, stop_{false} {}

            socket_listener::~socket_listener() {
                stop();
            }

            uint16_t socket_listener::port() const { return port_; }

            void socket_listener::stop() {
                stop_.store(true);
                if (thread_.joinable()) {
                    thread_.join();
                }
            }

            bool socket_listener::bind_and_listen() {
                if (fd_ < 0) { return false; }

                int yes = 1;
                ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

                struct sockaddr_in addr;
                std::memset(&addr, 0, sizeof(addr));
                addr.sin_family = AF_INET;
                addr.sin_addr.s_addr = INADDR_ANY;
                addr.sin_port = htons(port_);

                if (::bind(fd_, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
                    return false;
                }

                // when bound to port 0, learn the OS-assigned ephemeral port
                if (port_ == 0) {
                    struct sockaddr_in bound;
                    socklen_t len = sizeof(bound);
                    if (::getsockname(fd_, (struct sockaddr*)&bound, &len) == 0) {
                        port_ = ntohs(bound.sin_port);
                    }
                }

                return ::listen(fd_, 16) == 0;
            }

            int socket_listener::accept_ready(int timeout_ms) {
                if (fd_ < 0) { return -1; }

                fd_set read_set;
                FD_ZERO(&read_set);
                FD_SET(fd_, &read_set);

                struct timeval tv;
                tv.tv_sec = timeout_ms / 1000;
                tv.tv_usec = (timeout_ms % 1000) * 1000;

                int ready = ::select(fd_ + 1, &read_set, nullptr, nullptr, &tv);
                if (ready <= 0) { return -1; }   // timeout or error

                return ::accept(fd_, nullptr, nullptr);
            }
        }
    }
}
