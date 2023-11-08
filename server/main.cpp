#include "server.h"

#include <iostream>
#include <memory>

int main() {
    server_ptr server = std::make_shared<Server>();

    server->Start();

    std::cout << "Enter 'q' to quit" << std::endl;
    char word{};
    while (std::cin >> word) {
        if (word == 'q') {
            break;
        }
    }
    server->Stop();

    return 0;
}