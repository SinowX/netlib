//
// Created by sinow on 22-6-14.
//

#include "tcplistener.h"
#include <fcntl.h>
#include <unistd.h>

TcpListener::TcpListener(int fd, const SocketAddrV4 &addrV4) : Socket(fd, addrV4) {}

TcpListener::TcpListener(const SocketAddrV4 &addrV4) : Socket(addrV4) {
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1) {
        throw std::runtime_error(strerror(errno));
    }

    if (-1 == ::bind(_fd, (struct sockaddr *) &_addrv4.get(), sizeof(_addrv4.get()))) {
        throw std::runtime_error(strerror(errno));
    }

    if (-1 == ::listen(_fd, 1024)) {
        throw std::runtime_error(strerror(errno));
    }
}

TcpListener::TcpListener(const SocketV4FmtStr &addr)
        : TcpListener(SocketAddrV4(addr)) {}

TcpListener TcpListener::try_clone() const {
    int newfd = dup(_fd);
    if (-1 == newfd) {
        throw std::runtime_error(strerror(errno));
    }
    return {newfd, this->_addrv4};
}

std::tuple<TcpStream, SocketAddrV4> TcpListener::accept() const {
    int newfd = ::accept(_fd, nullptr, nullptr);
    if (-1 == newfd) {
        throw std::runtime_error(strerror(errno));
    }

    sockaddr_in addr{};
    socklen_t addr_len{sizeof(addr)};

    if (-1 == getpeername(newfd, (struct sockaddr *) &addr, &addr_len)) {
        throw std::runtime_error(strerror(errno));
    }

    return {TcpStream(newfd, SocketAddrV4(addr)), SocketAddrV4(addr)};
}
