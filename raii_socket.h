#ifndef OS_NET_RAII_SOCKET_H
#define OS_NET_RAII_SOCKET_H
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include "utils.h"

class raii_socket {
private:
    int sockfd;

public:
    raii_socket() {
        sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sockfd == -1) {
            throw_error("Couldn't open socket");
        }
    }
    raii_socket(int fd) : sockfd(fd) {}
    raii_socket(raii_socket const& other) = delete;
    raii_socket(raii_socket && other) : sockfd(other.sockfd) {
        other.sockfd = -1;
    }
    raii_socket& operator=(raii_socket&& other) noexcept {
        *this = std::move(other);
        return *this;
    }
    void bind(sockaddr_un& address) {
        int ret = ::bind(sockfd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
        if (ret == -1) {
            throw_error("Couldn't bind socket");
        }
    }
    void connect(sockaddr_un& address) {
        int ret = ::connect(sockfd, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr));
        if (ret == -1) {
            throw_error("Couldn't connect socket");
        }
    }
    raii_socket accept() {
        if (sockfd == -1) {
            throw_error("Couldn't accept socket");
        }
        int new_fd = ::accept(sockfd, nullptr, nullptr);
        return raii_socket(new_fd);
    }
    void listen() {
        int ret = ::listen(sockfd, 10);
        if (ret == -1) {
            throw_error("Couldn't listen socket");
        }
    }

    void send_sockfd(int _sockfd) {
        struct msghdr msg = {nullptr};
        char buf[CMSG_SPACE(sizeof(_sockfd))];
        memset(buf, 0, sizeof(buf));

        struct iovec io = {.iov_base = (void *) "", .iov_len = 1};
        msg.msg_iov = &io;
        msg.msg_iovlen = 1;
        msg.msg_control = buf;
        msg.msg_controllen = sizeof(buf);

        struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;
        cmsg->cmsg_len = CMSG_LEN(sizeof(_sockfd));

        memmove(CMSG_DATA(cmsg), &_sockfd, sizeof(_sockfd));

        msg.msg_controllen = cmsg->cmsg_len;

        if (sendmsg(sockfd, &msg, 0) < 0) {
            throw_error("Unable to send file descriptor");
        }
    }

    int get_sockfd() {
        msghdr msg = {nullptr};
        cmsghdr* cmsghdr;
        char buffer[CMSG_SPACE(sizeof(int))];
        char copy[512];
        bzero(buffer, sizeof(buffer));
        iovec iov = {.iov_base = &copy, .iov_len = sizeof(copy)};

        msg.msg_control = buffer;
        msg.msg_controllen = sizeof(buffer);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        if (recvmsg(sockfd, &msg, 0) < 0) {
            return -1;
        }

        cmsghdr = CMSG_FIRSTHDR(&msg);
        int ret_fd;
        memcpy(&ret_fd, (int *) CMSG_DATA(cmsghdr), sizeof(int));
        return ret_fd;
    }
    ~raii_socket() {
        if (sockfd != -1) {
            close(sockfd);
        }
    }

};

#endif //OS_NET_RAII_SOCKET_H
