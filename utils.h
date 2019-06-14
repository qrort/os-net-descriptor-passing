#ifndef OS_NET_UTILS_H
#define OS_NET_UTILS_H
#include <iostream>
#include <cstring>

void print_error(std::string const& message);

void throw_error(std::string const& message);

void write_message(int fd, std::string message);

std::string read_message(int fd);

#endif //OS_NET_UTILS_H
