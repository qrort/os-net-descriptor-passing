#ifndef OS_NET_CLIENT_H
#define OS_NET_CLIENT_H
#include "raii_socket.h"
#include <cstring>

class client {
public:
    client(char* socket_name);
    ~client() = default;
    void launch();

private:
    std::string server_addr;
    raii_socket client_socket;
};


#endif //OS_NET_CLIENT_H
