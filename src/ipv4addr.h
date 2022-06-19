//
// Created by sinow on 22-6-14.
//

#ifndef NETLIB_IPV4ADDR_H
#define NETLIB_IPV4ADDR_H

#include <arpa/inet.h>
#include <exception>

#include "src/types.h"

class Ipv4FmtErr : public std::exception {
    [[nodiscard]] const char *what() const noexcept override {
        return "Ipv4 Address Invalid Format";
    }
};

// forward declaration
class SocketAddrV4;

class Ipv4Addr {
public:

    Ipv4Addr(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
            : Ipv4Addr(
            std::to_string(a) + '.' + std::to_string(b) + '.' + std::to_string(c) + '.' + std::to_string(d)) {
    }

    explicit Ipv4Addr(const Ipv4FmtStr &ip) {
        // 0 indicates failure
        if (0 == inet_aton(ip.c_str(), &_internal)) {
            throw Ipv4FmtErr{};
        }
    }

    [[nodiscard]] struct in_addr const &get() const {
        return _internal;
    }

    struct in_addr &get() {
        return _internal;
    }

public:
    [[nodiscard]] Ipv4Tuple octets() const {
        auto n = _internal.s_addr;
        uint8_t a, b, c, d;
        a = n;
        n >>= 8;
        b = n;
        n >>= 8;
        c = n;
        n >>= 8;
        d = n;
        return {a, b, c, d};
    }

    [[nodiscard]] Ipv4FmtStr str() const {
        return {inet_ntoa(_internal)};
    }

    [[nodiscard]] bool is_unspecified() const {
        return UNSPECIFIED._internal.s_addr == this->_internal.s_addr;
    }

    [[nodiscard]] bool is_localhost() const {
        return LOCALHOST._internal.s_addr == this->_internal.s_addr;

    }

    [[nodiscard]] bool is_broadcast() const {
        return BROADCAST._internal.s_addr == this->_internal.s_addr;
    }

    // private address: 10.0.0.0/8, 172.16.0.0/12, 192.168.0.0/16
    [[nodiscard]] bool is_private() const {
        static const Ipv4Addr a{10, 0, 0, 0};
        static const Ipv4Addr b{172, 16, 0, 0};
        static const Ipv4Addr c{192, 168, 0, 0};
        return (a._internal.s_addr & this->_internal.s_addr) == a._internal.s_addr
               || (b._internal.s_addr & this->_internal.s_addr) == b._internal.s_addr
               || (c._internal.s_addr & this->_internal.s_addr) == c._internal.s_addr;
    }

public:
    static Ipv4Addr const LOCALHOST;
    static Ipv4Addr const BROADCAST;
    static Ipv4Addr const UNSPECIFIED;

private:
    struct in_addr _internal{};
};

inline Ipv4Addr const Ipv4Addr::LOCALHOST{127, 0, 0, 1};
inline Ipv4Addr const Ipv4Addr::BROADCAST{255, 255, 255, 255};
inline Ipv4Addr const Ipv4Addr::UNSPECIFIED{0, 0, 0, 0};

#endif //NETLIB_IPV4ADDR_H