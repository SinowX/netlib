//
// Created by sinow on 22-6-14.
//

#ifndef NETLIB_TCPLISTENER_H
#define NETLIB_TCPLISTENER_H

#include "tcpstream.h"

class TcpListener : public Socket {
private:
//    SocketAddrV4 _addrv4;
//    int _fd{};

    TcpListener(int fd, SocketAddrV4 const &addrV4);

public:
    explicit TcpListener(SocketAddrV4 const &addrV4);

    explicit TcpListener(SocketV4FmtStr const &addr);

    [[nodiscard]] TcpListener try_clone() const;

    [[nodiscard]] std::tuple<TcpStream, SocketAddrV4> accept() const;

//public: // delete base class function
//    void peer_addr() = delete;
//    void

};


#endif //NETLIB_TCPLISTENER_H
