#include "server.h"
#include <iostream>

int main() try {
    HTTPServer server(8080);
    server.start();
    return 0;
}
catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
}