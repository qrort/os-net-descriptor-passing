#ifndef OS_NET_SERVER_H
#define OS_NET_SERVER_H


#include "raii_socket.h"

class server {
public:
    server() = delete;
    server(char* socket_name);
    ~server();
    server(const server& other) = delete;
    server& operator=(const server& other) = delete;
    void launch();
private:
    std::string server_addr;
    raii_socket server_socket;
    bool down = false;
    int pipe_in[2] = {-1, -1}, pipe_out[2] = {-1, -1};
};


#endif //OS_NET_SERVER_H
