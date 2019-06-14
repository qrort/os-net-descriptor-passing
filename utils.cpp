#include "utils.h"
#include <sys/socket.h>
#include <zconf.h>

void print_error(std::string const& message) {
    std::cerr << message << ": " << strerror(errno) << '\n';
}

void throw_error(std::string const& message) {
    throw std::runtime_error(message + ": " + strerror(errno));
}

void write_message(int fd, std::string message) {
    message += '\0';
    size_t ptr = 0;
    while (ptr < message.size()) {
        ssize_t len = write(fd, message.c_str() + ptr, message.size() - ptr);
        if (len == -1) {
            throw_error("Couldn't write data to file descriptor");
        }
        ptr += len;
    }
}

std::string read_message(int fd) {
    size_t BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE + 1];
    std::string res;
    while (true) {
        ssize_t len = read(fd, &buffer, BUFFER_SIZE);
        if (len == -1) {
            throw_error("Couldn't read data from file descriptor");
        }
        buffer[BUFFER_SIZE] = '\0';
        if (!len) break;
        res.append(buffer, len);
        if (buffer[len - 1] == '\0') {
            res.pop_back();
            break;
        }
    }
    return res;
}