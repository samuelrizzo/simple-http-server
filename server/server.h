#pragma once

#include <cstdint>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
using socket_t = SOCKET;
#else
using socket_t = int;
#endif

class HTTPServer {
public:
    explicit HTTPServer(uint16_t port);
    ~HTTPServer();

    void start();

    HTTPServer(const HTTPServer&) = delete;
    HTTPServer& operator=(const HTTPServer&) = delete;

private:
    void initializeWSA();
    void cleanupWSA();
    void handleClient(socket_t client);
    std::string createResponse() const;

    socket_t server_socket;
    const uint16_t port;
};