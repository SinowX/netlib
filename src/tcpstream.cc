//
// Created by sinow on 22-6-14.
//

#include "src/tcpstream.h"
#include <unistd.h>


TcpStream::TcpStream(int fd, const SocketAddrV4 &addrV4)
        : Socket(fd, addrV4) {}

TcpStream::TcpStream(const SocketAddrV4 &addrV4)
        : Socket(addrV4) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == fd) {
        throw std::runtime_error(strerror(errno));
    }

    _fd = fd;

    if (-1 == ::bind(_fd, (struct sockaddr *) &_addrv4.get(), sizeof(_addrv4.get()))) {
        throw std::runtime_error(strerror(errno));
    }
}

TcpStream::TcpStream(const SocketV4FmtStr &addr) : TcpStream(SocketAddrV4(addr)) {}

TcpStream TcpStream::connect(SocketAddrV4 const &addrV4) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == fd) {
        throw std::runtime_error(strerror(errno));
    }

    if (-1 == ::connect(fd, (struct sockaddr *) &addrV4.get(), sizeof(addrV4.get()))) {
        auto errcode = errno;
        close(fd); // forbid fd leak
        throw std::runtime_error(strerror(errcode));
    }

    struct sockaddr_in addr{};
    socklen_t addr_len{sizeof(addr)};

    if (-1 == getsockname(fd, (struct sockaddr *) &addr, &addr_len)) {
        auto errcode = errno;
        close(fd); // forbid fd leak
        throw std::runtime_error(strerror(errcode));
    }

    return {fd, SocketAddrV4(addr)};
}

TcpStream TcpStream::connect(const SocketV4FmtStr &addr) {
    return connect(SocketAddrV4(addr));
}

// to do
TcpStream TcpStream::connect_timeout(const SocketAddrV4 &, std::chrono::nanoseconds) {

}

TcpStream TcpStream::try_clone() {
    int newfd = dup(_fd);
    if (-1 == newfd) {
        throw std::runtime_error(strerror(errno));
    }
    return {newfd, this->_addrv4};
}

void TcpStream::set_linger(std::chrono::nanoseconds dura) const {
    int sec = dura.count() / 1000000000;
    struct linger value{.l_onoff{1}, .l_linger{sec}};
    if (-1 == setsockopt(_fd, SOL_SOCKET, SO_LINGER, &value, sizeof(value))) {
        throw std::runtime_error(strerror(errno));
    }

}

std::chrono::nanoseconds TcpStream::linger() const {
    struct linger value{};
    socklen_t len{sizeof(value)};
    if (-1 == getsockopt(_fd, SOL_SOCKET, SO_LINGER, &value, &len)) {
        throw std::runtime_error(strerror(errno));
    }
    return std::chrono::seconds{value.l_linger};
}


