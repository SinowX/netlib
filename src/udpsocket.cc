//
// Created by sinow on 22-6-14.
//

#include "udpsocket.h"
#include <unistd.h>

UdpSocket::UdpSocket() {
    _fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (_fd == -1) {
        throw std::runtime_error(strerror(errno));
    }
}

UdpSocket::UdpSocket(int fd, const SocketAddrV4 &addrV4)
        : Socket(fd, addrV4) {}

UdpSocket::UdpSocket(const SocketAddrV4 &addrV4)
        : Socket(addrV4) {
    _fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (_fd == -1) {
        throw std::runtime_error(strerror(errno));
    }

    if (-1 == bind(_fd, (struct sockaddr *) &_addrv4.get(), sizeof(_addrv4.get()))) {
        throw std::runtime_error(strerror(errno));
    }
}

UdpSocket::UdpSocket(const SocketV4FmtStr &addr)
        : UdpSocket(SocketAddrV4(addr)) {}

UdpSocket UdpSocket::try_clone() const {
    int newfd = dup(_fd);
    if (-1 == newfd) {
        throw std::runtime_error(strerror(errno));
    }
    return {newfd, this->_addrv4};
}

std::tuple<ssize_t, SocketAddrV4> UdpSocket::recv_from(BufType buf) const {
    struct sockaddr_in peer_addr{};
    socklen_t addr_len{sizeof(peer_addr)};
    auto sz = recvfrom(_fd, buf.data(), buf.size(), 0, (struct sockaddr *) &peer_addr, &addr_len);
    if (sz == -1) {
        throw std::runtime_error(strerror(errno));
    }
    buf.resize(sz);
    return {sz, SocketAddrV4(peer_addr)};
}

std::tuple<ssize_t, SocketAddrV4> UdpSocket::peek_from(BufType buf) const {
    struct sockaddr_in peer_addr{};
    socklen_t addr_len{sizeof(peer_addr)};
    auto sz = recvfrom(_fd, buf.data(), buf.size(), MSG_PEEK, (struct sockaddr *) &peer_addr, &addr_len);
    if (sz == -1) {
        throw std::runtime_error(strerror(errno));
    }
    buf.resize(sz);
    return {sz, SocketAddrV4(peer_addr)};
}

ssize_t UdpSocket::send_to(const BufType &buf, SocketAddrV4 addr) const {
    auto sz = sendto(_fd, buf.data(), buf.size(), 0, (struct sockaddr *) &addr.get(), sizeof(addr.get()));
    if (sz == -1) {
        throw std::runtime_error(strerror(errno));
    }
    return sz;
}

void UdpSocket::set_broadcast(bool on) const {
    int flag{on};
    if (-1 == setsockopt(_fd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag))) {
        throw std::runtime_error(strerror(errno));
    }
}

bool UdpSocket::broadcast() const {
    int flag{};
    socklen_t len{sizeof(flag)};
    if (-1 == getsockopt(_fd, SOL_SOCKET, SO_BROADCAST, &flag, &len)) {
        throw std::runtime_error(strerror(errno));
    }
    return 0 != flag;
}

void UdpSocket::set_multicast_loop_v4(bool on) const {
    int flag{on};
    if (-1 == setsockopt(_fd, IPPROTO_IP, IP_MULTICAST_LOOP, &flag, sizeof(flag))) {
        throw std::runtime_error(strerror(errno));
    }
}

bool UdpSocket::multicast_loop_v4() const {
    int flag{};
    socklen_t len{sizeof(flag)};
    if (-1 == getsockopt(_fd, IPPROTO_IP, IP_MULTICAST_LOOP, &flag, &len)) {
        throw std::runtime_error(strerror(errno));
    }
    return 0 != flag;
}

void UdpSocket::set_multicast_ttl_v4(uint32_t ttl) const {
    if (-1 == setsockopt(_fd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl))) {
        throw std::runtime_error(strerror(errno));
    }
}

uint32_t UdpSocket::multicast_ttl_v4() const {
    uint32_t res{};
    socklen_t len{sizeof(res)};
    if (-1 == getsockopt(_fd, IPPROTO_IP, IP_MULTICAST_TTL, &res, &len)) {
        throw std::runtime_error(strerror(errno));
    }
    return res;
}

void UdpSocket::join_multicast_v4(const Ipv4Addr &multiaddr, int const if_index) {
    struct ip_mreqn grp{.imr_multiaddr{multiaddr.get()}, .imr_address{
            _addrv4.get().sin_addr}, .imr_ifindex = if_index};
    if (-1 == setsockopt(_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &grp, sizeof(grp))) {
        throw std::runtime_error(strerror(errno));
    }
}

void UdpSocket::leave_multicast_v4(const Ipv4Addr &multiaddr, int if_index) {
    struct ip_mreqn grp{.imr_multiaddr{multiaddr.get()}, .imr_address{
            _addrv4.get().sin_addr}, .imr_ifindex = if_index};
    if (-1 == setsockopt(_fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &grp, sizeof(grp))) {
        throw std::runtime_error(strerror(errno));
    }
}

void UdpSocket::connect(SocketAddrV4 addr) const {
    if (-1 == ::connect(_fd, (struct sockaddr *) &addr.get(), sizeof(addr.get()))) {
        throw std::runtime_error(strerror(errno));
    }
}



