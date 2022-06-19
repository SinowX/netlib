//
// Created by sinow on 22-6-14.
//

#ifndef NETLIB_TCPSTREAM_H
#define NETLIB_TCPSTREAM_H

#include "src/socket.h"

class TcpListener;

class TcpStream : public Socket {
private:
    friend TcpListener;

//    SocketAddrV4 _addrv4;
//    int _fd{};

    // for private and TcpListern Use
    TcpStream(int fd, SocketAddrV4 const &addrV4);

public:
    // bind this addr
    explicit TcpStream(SocketAddrV4 const &addrV4);

    explicit TcpStream(SocketV4FmtStr const& addr);

    static TcpStream connect(SocketAddrV4 const &);

    static TcpStream connect(SocketV4FmtStr const &);

    static TcpStream connect_timeout(SocketAddrV4 const &, std::chrono::nanoseconds);

public:

    TcpStream try_clone();

    void set_linger(std::chrono::nanoseconds) const;

    [[nodiscard]] std::chrono::nanoseconds linger() const;

};

#endif //NETLIB_TCPSTREAM_H
