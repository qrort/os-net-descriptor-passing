#ifndef OS_NET_CLIENT_H
#define OS_NET_CLIENT_H
#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include "../raii_socket.h"

class client {
public:
    static const size_t BUFFER_SIZE;
    client() = delete;
    client(char* socket_name);
    ~client() = default;
    std::string send_and_receive(std::string const& message);
    raii_socket open_connection();
private:
    struct sockaddr_un address;
};


#endif //OS_NET_CLIENT_H
