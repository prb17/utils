#pragma once

#include <string>
#include <cstddef>
#include <cstdint>
#include <atomic>
#include <thread>

namespace prb17 {
    namespace utils {
        namespace sockets {

            /**
             * @brief RAII wrapper around a POSIX TCP socket file descriptor.
             *
             * Owns the descriptor and closes it on destruction. Move-only (a socket
             * is a unique resource). Provides byte and newline-framed string I/O.
             */
            class socket {
                protected:
                    int fd_;

                public:
                    socket();                       // create a TCP socket
                    explicit socket(int fd);        // adopt an existing descriptor
                    socket(socket&& other) noexcept;
                    socket& operator=(socket&& other) noexcept;
                    socket(const socket&) = delete;
                    socket& operator=(const socket&) = delete;
                    virtual ~socket();

                    bool valid() const;
                    int fd() const;

                    // send/recv exactly n bytes; false on error or early close
                    bool send_bytes(const char* buffer, size_t n);
                    bool recv_bytes(char* buffer, size_t n);

                    // newline-framed strings (terminator is written/consumed, not stored)
                    bool send_string(const std::string& str, char terminator = '\n');
                    std::string recv_string(char terminator = '\n');

                    bool shutdown();
                    void close();
            };

            /**
             * @brief A socket that can connect to a listening peer.
             */
            class socket_connecter : public socket {
                public:
                    socket_connecter();
                    bool connect(const std::string& host, uint16_t port);
            };

            /**
             * @brief A socket that binds a port and accepts connections on its own
             *      thread, invoking a handler with each accepted socket.
             *
             * Construct with port 0 to let the OS choose an ephemeral port, then read
             * the actual port via port(). stop() (also called by the destructor) ends
             * the accept loop and joins the thread.
             */
            class socket_listener : public socket {
                private:
                    uint16_t port_;
                    std::atomic<bool> stop_;
                    std::thread thread_;

                    bool bind_and_listen();
                    // wait up to timeout_ms for a connection; returns a client fd or -1
                    int accept_ready(int timeout_ms);

                public:
                    socket_listener(uint16_t port);
                    socket_listener(const socket_listener&) = delete;
                    socket_listener& operator=(const socket_listener&) = delete;
                    ~socket_listener();

                    uint16_t port() const;
                    void stop();

                    /**
                     * @brief binds/listens, then runs an accept loop on a background
                     *      thread. Each accepted connection is passed to handler as an
                     *      owned socket. Returns false if bind/listen failed.
                     */
                    template<typename Handler>
                    bool start(Handler handler) {
                        if (!bind_and_listen()) { return false; }
                        stop_.store(false);
                        thread_ = std::thread([this, handler]() mutable {
                            while (!stop_.load()) {
                                int client_fd = accept_ready(100);
                                if (client_fd >= 0) {
                                    socket client{client_fd};
                                    handler(std::move(client));
                                }
                            }
                        });
                        return true;
                    }
            };
        }
    }
}
