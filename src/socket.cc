//
// Created by sinow on 22-6-16.
//

#include "socket.h"
#include <unistd.h>
#include <fcntl.h>

Socket::Socket(const SocketAddrV4 &addrV4)
        : _addrv4{addrV4} {}

Socket::~Socket() {
    shutdown(Shutdown::Write);
}

Socket::Socket(const SocketV4FmtStr &addr)
        : Socket(SocketAddrV4(addr)) {}

Socket::Socket(Socket &&other) noexcept
        : _fd{other._fd}, _addrv4{other._addrv4} {
    other._fd = -1;
}

Socket &Socket::operator=(Socket &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    close(this->_fd);
    this->_fd = other._fd;
    this->_addrv4 = other._addrv4;
    other._fd = -1;
    return *this;
}

ssize_t Socket::send(const BufType &buf) const {
    auto sz = ::send(_fd, buf.data(), buf.size(), 0);
    if (-1 == sz) {
        throw std::runtime_error(strerror(errno));
    }
    return sz;
}

ssize_t Socket::recv(BufType &buf) const {
    auto sz = ::recv(_fd, buf.data(), buf.size(), 0);
    if (-1 == sz) {
        throw std::runtime_error(strerror(errno));
    }
    buf.resize(sz);
    return sz;
}

ssize_t Socket::peek(BufType &buf) const {
    auto sz = ::recv(_fd, buf.data(), buf.size(), MSG_PEEK);
    if (-1 == sz) {
        throw std::runtime_error(strerror(errno));
    }
    buf.resize(sz);
    return sz;
}

void Socket::shutdown(Shutdown how) const noexcept {
    int flag{};
    switch (how) {
        case Shutdown::Read: {
            flag = SHUT_RD;
            break;
        }
        case Shutdown::Write: {
            flag = SHUT_WR;
            break;
        }
        case Shutdown::Both: {
            flag = SHUT_RDWR;
            break;
        }
        default: {
            abort(); // unexpected
        }
    }
    ::shutdown(_fd, flag);
}

SocketAddrV4 Socket::peer_addr() const {
    sockaddr_in addr{};
    socklen_t addr_len{};

    if (-1 == getpeername(_fd, (struct sockaddr *) (&addr), &addr_len)) {
        throw std::runtime_error(strerror(errno));
    }
    return SocketAddrV4(addr);
}

SocketAddrV4 Socket::local_addr() const {
    return _addrv4;
}

static int get_fd_flag(int fd) {
    int flag{fcntl(fd, F_GETFL)};
    if (-1 == flag) {
        throw std::runtime_error(strerror(errno));
    }
    return flag;
}

static void set_fd_flag(int fd, int flag) {
    if (-1 == fcntl(fd, F_SETFL, flag)) {
        throw std::runtime_error(strerror(errno));
    }
}

void Socket::set_nonblocking(bool on) const {
    int flag{get_fd_flag(_fd) & (on ? O_NONBLOCK : (~O_NONBLOCK))};
    set_fd_flag(_fd, flag);
}

bool Socket::nonblocking() const {
    return 0 != (get_fd_flag(_fd) & O_NONBLOCK);
}

// 10^9 constexpr uint32_t
static constexpr uint32_t TEN_9 = 1000000000;

// 10^3 constexpr uint32_t
static constexpr uint32_t TEN_3 = 1000;

void Socket::set_read_timeout(Duration dura) const {
    // N nanoseconds = N / TEN_9 seconds + (N % TEN_9) / TEN_3 microseconds
    struct timeval tmval{.tv_sec= dura.count() / TEN_9, .tv_usec = (dura.count() % TEN_9) / TEN_3};
    if (-1 == setsockopt(_fd, SOL_SOCKET, SO_RCVTIMEO, &tmval, sizeof(tmval))) {
        throw std::runtime_error(strerror(errno));
    }
}

void Socket::set_write_timeout(Duration dura) const {
    // N nanoseconds = N / TEN_9 seconds + (N % TEN_9) / TEN_3 microseconds
    struct timeval tmval{.tv_sec= dura.count() / TEN_9, .tv_usec = (dura.count() % TEN_9) / TEN_3};
    if (-1 == setsockopt(_fd, SOL_SOCKET, SO_SNDTIMEO, &tmval, sizeof(tmval))) {
        throw std::runtime_error(strerror(errno));
    }
}

Duration Socket::read_timeout() const {
    struct timeval tmval{};
    socklen_t len{sizeof(tmval)};
    if (-1 == getsockopt(_fd, SOL_SOCKET, SO_RCVTIMEO, &tmval, &len)) {
        throw std::runtime_error(strerror(errno));
    }
    return std::chrono::nanoseconds{tmval.tv_sec * TEN_9 + tmval.tv_usec * TEN_3};
}

Duration Socket::write_timeout() const {
    struct timeval tmval{};
    socklen_t len{sizeof(tmval)};
    if (-1 == getsockopt(_fd, SOL_SOCKET, SO_SNDTIMEO, &tmval, &len)) {
        throw std::runtime_error(strerror(errno));
    }
    return std::chrono::nanoseconds{tmval.tv_sec * TEN_9 + tmval.tv_usec * TEN_3};
}

void Socket::set_ttl(uint32_t ttl) const {
    if (-1 == setsockopt(_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl))) {
        throw std::runtime_error(strerror(errno));
    }
}

uint32_t Socket::ttl() const {
    uint32_t res{};
    socklen_t len{sizeof(res)};
    if (-1 == getsockopt(_fd, IPPROTO_IP, IP_TTL, &res, &len)) {
        throw std::runtime_error(strerror(errno));
    }
    return res;
}

error_t Socket::take_error() const {
    error_t err{};
    socklen_t len{sizeof(err)};
    if (-1 == getsockopt(_fd, SOL_SOCKET, SO_ERROR, &err, &len)) {
        throw std::runtime_error(strerror(errno));
    }
    return err;
}

