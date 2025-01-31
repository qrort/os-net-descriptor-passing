#include "client.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Address argument expected" << std::endl;
    } else {
        try {
            client client(argv[1]);
            std::cout << "Echo client. https://tools.ietf.org/html/rfc862\n"
                         "Type \"stop\" to stop server. Type \"end\" to end session" << std::endl;
            client.launch();
            std::cout << "Work done";
        } catch (std::runtime_error &e) {
            std::cout << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}