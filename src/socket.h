//
// Created by sinow on 22-6-16.
//

#ifndef NETLIB_SOCKET_H
#define NETLIB_SOCKET_H

#include <cstring>
#include <stdexcept>
#include "src/socketaddrv4.h"

enum class Shutdown {
    Read,
    Write,
    Both,
};

class Socket {
protected: // internal data

    SocketAddrV4 _addrv4{};
    int _fd{-1};

protected: // for private use

    Socket() = default;

    Socket(int fd, SocketAddrV4 const &addrV4)
            : _fd{fd}, _addrv4{addrV4} {}

public: // open constructor

    explicit Socket(SocketAddrV4 const &addrV4);

    explicit Socket(SocketV4FmtStr const &addr);

public: // big five

    Socket(Socket const &) = delete;

    void operator=(Socket const &) = delete;

    Socket(Socket &&other) noexcept;

    Socket &operator=(Socket &&) noexcept;

    virtual ~Socket();

public: // connection management

    // send data if connected, otherwise fail
    [[nodiscard]] ssize_t send(BufType const &buf) const;

    // receive data if connected, otherwise fail
    ssize_t recv(BufType &buf) const;

    // peek data if connected, otherwise fail
    ssize_t peek(BufType &buf) const;

    void shutdown(Shutdown how) const noexcept;

public: // socket options

    [[nodiscard]] SocketAddrV4 peer_addr() const;

    [[nodiscard]] SocketAddrV4 local_addr() const;

    void set_nonblocking(bool on) const;

    [[nodiscard]] bool nonblocking() const;

    void set_read_timeout(Duration) const;

    [[nodiscard]] std::chrono::nanoseconds read_timeout() const;

    void set_write_timeout(Duration) const;

    [[nodiscard]] std::chrono::nanoseconds write_timeout() const;

    void set_ttl(uint32_t ttl) const;

    [[nodiscard]] uint32_t ttl() const;

    // return value need to rethink
    // SO_ERROR option
    [[nodiscard]] error_t take_error() const;

};


#endif //NETLIB_SOCKET_H
