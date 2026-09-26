#include <string>

#include "validator.hh"
#include "logger.hh"

#include "endpoint.hh"
#include "message.hh"
#include "socket.hh"

using namespace prb17::utils::sockets;
static prb17::utils::logger logger{"socket_test"};

// endpoint(address, port) renders as "address:port" and parses back
bool testEndpointRoundTrip(prb17::utils::parsers::json_parser jp) {
    std::string address = jp.as_string("address");
    uint16_t port = (uint16_t)jp.as_int("port");
    std::string expected = jp.as_string("expected");

    endpoint ep{address, port};
    bool result = (ep.to_string() == expected)
               && (endpoint::from_string(expected) == ep);
    logger.debug("endpoint '{}' expected '{}'", ep.to_string(), expected);
    return result;
}

// from_string extracts address and port
bool testEndpointFromString(prb17::utils::parsers::json_parser jp) {
    endpoint ep = endpoint::from_string(jp.as_string("input"));
    return (ep.address == jp.as_string("expected_address"))
        && (ep.port == (uint16_t)jp.as_int("expected_port"));
}

// a message survives serialization and re-parsing intact
bool testMessageRoundTrip(prb17::utils::parsers::json_parser jp) {
    message m{
        endpoint{jp.as_string("src_address"), (uint16_t)jp.as_int("src_port")},
        endpoint{jp.as_string("dst_address"), (uint16_t)jp.as_int("dst_port")},
        jp.as_string("type"),
        jp.as_string("body")
    };
    message parsed = message::from_string(m.to_string());
    logger.debug("message serialized: '{}'", m.to_string());
    return parsed == m;
}

// a client and a threaded listener exchange a line over the loopback interface
bool testLoopbackEcho(prb17::utils::parsers::json_parser jp) {
    std::string body = jp.as_string("body");

    socket_listener listener{0}; // ephemeral port
    bool started = listener.start([](socket&& client) {
        std::string line = client.recv_string();
        client.send_string(line);
    });
    if (!started) { return false; }

    socket_connecter client{};
    bool connected = client.connect("127.0.0.1", listener.port());
    if (!connected) { listener.stop(); return false; }

    client.send_string(body);
    std::string reply = client.recv_string();
    listener.stop();

    logger.debug("loopback echo sent '{}' got '{}'", body, reply);
    return reply == body;
}

// a full message round-trips over the wire, framed as a single line
bool testLoopbackMessage(prb17::utils::parsers::json_parser jp) {
    message original{
        endpoint{"127.0.0.1", 1}, endpoint{"127.0.0.1", 2},
        jp.as_string("type"), jp.as_string("body")
    };

    socket_listener listener{0};
    bool started = listener.start([](socket&& client) {
        message m = message::from_string(client.recv_string());
        client.send_string(m.to_string()); // echo the parsed-and-reserialized message
    });
    if (!started) { return false; }

    socket_connecter client{};
    if (!client.connect("127.0.0.1", listener.port())) { listener.stop(); return false; }

    client.send_string(original.to_string());
    message reply = message::from_string(client.recv_string());
    listener.stop();

    logger.debug("loopback message body '{}' -> '{}'", original.body, reply.body);
    return reply == original;
}

#define MIN_NUM_ARGS 2
int main(int argc, char** argv) {
    if (argc < MIN_NUM_ARGS) {
        throw prb17::utils::exception("This test requires a config file to be provided");
    }
    prb17::utils::structures::array<std::string> test_files{};
    for (int i=1; i<argc; i++) {
        test_files.add(&argv[i][0]);
    }
    prb17::utils::validator validator{test_files};

    validator.add_test("testEndpointRoundTrip", &testEndpointRoundTrip, "");
    validator.add_test("testEndpointFromString", &testEndpointFromString, "");
    validator.add_test("testMessageRoundTrip", &testMessageRoundTrip, "");
    validator.add_test("testLoopbackEcho", &testLoopbackEcho, "");
    validator.add_test("testLoopbackMessage", &testLoopbackMessage, "");

    logger.info("Starting validation tests of socket_tests");
    validator.validate();
    logger.info("Finished validation tests of socket_tests");
}
