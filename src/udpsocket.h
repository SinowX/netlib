//
// Created by sinow on 22-6-14.
//

#ifndef NETLIB_UDPSOCKET_H
#define NETLIB_UDPSOCKET_H

#include "src/socket.h"

class UdpSocket : public Socket {
private:
//    SocketAddrV4 _addrv4;
//    int _fd{};

    // for private use
    UdpSocket(int fd, SocketAddrV4 const &addrV4);

public:
    UdpSocket();

    explicit UdpSocket(SocketAddrV4 const &addrV4);

    explicit UdpSocket(SocketV4FmtStr const &addr);

    // return received size and datagram source addr
    [[nodiscard]] std::tuple<ssize_t, SocketAddrV4> recv_from(BufType buf) const;

    [[nodiscard]] std::tuple<ssize_t, SocketAddrV4> peek_from(BufType buf) const;

    [[nodiscard]] ssize_t send_to(BufType const &buf, SocketAddrV4 addr) const;

    // rare to use.
    [[nodiscard]] UdpSocket try_clone() const;

    // rare to use.
    void set_broadcast(bool on) const;

    // rare to use.
    [[nodiscard]] bool broadcast() const;

    // rare to use.
    void set_multicast_loop_v4(bool on) const;

    // rare to use.
    [[nodiscard]] bool multicast_loop_v4() const;

    void set_multicast_ttl_v4(uint32_t ttl) const;

    [[nodiscard]] uint32_t multicast_ttl_v4() const;

    void join_multicast_v4(Ipv4Addr const &multiaddr, int if_index = 0);

    void leave_multicast_v4(Ipv4Addr const &multiaddr, int if_index = 0);

    void connect(SocketAddrV4 addr) const;
};

#endif //NETLIB_UDPSOCKET_H
