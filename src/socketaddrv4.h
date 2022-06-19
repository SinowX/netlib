//
// Created by sinow on 22-6-14.
//

#ifndef NETLIB_SOCKETADDRV4_H
#define NETLIB_SOCKETADDRV4_H

#include "src/ipv4addr.h"
#include <netinet/in.h>
#include <iostream>

class SocketAddrV4 {
public:
    SocketAddrV4() = default;

    explicit SocketAddrV4(sockaddr_in const &addr)
            : _internal{addr} {}

    SocketAddrV4(Ipv4Addr const &addr, uint16_t port) {
        _internal.sin_family = AF_INET;
        _internal.sin_addr = addr.get();
        _internal.sin_port = htons(port);
    }

    explicit SocketAddrV4(SocketV4FmtStr const &addr_str) {
        auto delim = addr_str.find(':');
        if (delim == std::string::npos) {
            throw Ipv4FmtErr{};
        }

        auto idx = delim + 1;

        _internal.sin_family = AF_INET;
        auto &&ip = addr_str.substr(0, delim);
        _internal.sin_addr = Ipv4Addr(ip).get();
//        auto port = std::stoul(addr_str, &idx);
        auto &&port = addr_str.substr(delim + 1, addr_str.size());
        _internal.sin_port = htons(std::stoul(port));

        std::cout << "ip: " << inet_ntoa(_internal.sin_addr) << std::endl;
        std::cout << "port: " << ntohs(_internal.sin_port) << std::endl;

    }

    [[nodiscard]] struct sockaddr_in const &get() const {
        return _internal;
    }

    struct sockaddr_in &get() {
        return _internal;
    }

    [[nodiscard]] Ipv4Addr ip() const {
        return Ipv4Addr{inet_ntoa(_internal.sin_addr)};
    }

    void set_ip(Ipv4Addr const &addr) {
        _internal.sin_addr = addr.get();
    }

    [[nodiscard]] uint16_t port() const {
        return ntohs(_internal.sin_port);
    }

    void set_port(uint16_t port) {
        _internal.sin_port = htons(port);
    }

private:
    struct sockaddr_in _internal{};
};


#endif //NETLIB_SOCKETADDRV4_H
