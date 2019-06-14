#include "server.h"
#include "utils.h"
#include <unistd.h>

server::server(char *socket_name) : server_addr(socket_name), server_socket() {
    struct sockaddr_un address;
    memset(&address, 0, sizeof(sockaddr_un));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, socket_name, sizeof(address.sun_path) - 1);
    server_socket.bind(address);
    server_socket.listen();
}


void server::launch() {
    raii_socket client_socket = server_socket.accept();

    if (::pipe(pipe_in) == -1 || ::pipe(pipe_out) == -1) {
        throw_error("Couldn't open pipe");
    }

    std::cerr << "sending sockfd: " << pipe_in[0] << ' ' << pipe_out[1] << '\n';
    client_socket.send_sockfd(pipe_in[0]);
    client_socket.send_sockfd(pipe_out[1]);
    std::cout << "Client connected" << '\n';

    int in_fd = pipe_out[0];
    int out_fd = pipe_in[1];
    while (true) {
        std::string msg = read_message(in_fd);
        std::cout << "Server received " << msg << '\n';

        write_message(out_fd, msg);
        std::cout << "Server sent " << msg << '\n';

        if (msg == "stop") {
            std::cout << "Server stopped" << '\n';
            break;
        }
    }
}


server::~server() {
    for (int i = 0; i < 2; i++) {
        if (close(pipe_in[i]) == -1 || close(pipe_out[i]) == -1) {
            throw_error("Couldn't close pipe");
        }
    }
    unlink(server_addr.c_str());
}
