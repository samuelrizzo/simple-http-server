#include "server.h"
#include <iostream>
#include <stdexcept>

#ifdef _WIN32
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define CLOSE_SOCKET closesocket
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define CLOSE_SOCKET close
#endif

HTTPServer::HTTPServer(uint16_t port) : port(port) {
    initializeWSA();

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        cleanupWSA();
        throw std::runtime_error("Socket creation failed");
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_socket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
        CLOSE_SOCKET(server_socket);
        cleanupWSA();
        throw std::runtime_error("Bind failed");
    }
}

HTTPServer::~HTTPServer() {
    CLOSE_SOCKET(server_socket);
    cleanupWSA();
}

void HTTPServer::start() {
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
        throw std::runtime_error("Listen failed");
    }

    std::cout << "Server running on port " << port << "\n";

    while (true) {
        socket_t client = accept(server_socket, nullptr, nullptr);
        if (client == INVALID_SOCKET) continue;
        handleClient(client);
    }
}

void HTTPServer::handleClient(socket_t client) {
    char buffer[1024]{};
#ifdef _WIN32
    recv(client, buffer, sizeof(buffer), 0);
#else
    read(client, buffer, sizeof(buffer));
#endif

    std::string request(buffer);
    if (request.find("GET / HTTP") != std::string::npos) {
        std::string response = createResponse();
#ifdef _WIN32
        send(client, response.c_str(), static_cast<int>(response.length()), 0);
#else
        write(client, response.c_str(), response.length());
#endif
    }

    CLOSE_SOCKET(client);
}

std::string HTTPServer::createResponse() const {
    return "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Connection: close\r\n\r\n"
        "{\"message\": \"hello world\"}";
}

void HTTPServer::initializeWSA() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
#endif
}

void HTTPServer::cleanupWSA() {
#ifdef _WIN32
    WSACleanup();
#endif
}