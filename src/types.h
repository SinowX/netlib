//
// Created by sinow on 22-6-14.
//

#ifndef NETLIB_TYPES_H
#define NETLIB_TYPES_H

#include <vector>
#include <chrono>
#include <string>
#include <tuple>

using BufType = std::vector<char>;

// like: 127.0.0.1
using Ipv4FmtStr = std::string;
using Ipv4Tuple = std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>;

// like: 127.0.0.1:5000
using SocketV4FmtStr = std::string;

using Duration = std::chrono::nanoseconds;

#endif //NETLIB_TYPES_H
