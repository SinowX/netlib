#include "src/tcpstream.h"
#include <iostream>
#include <iterator>

int main() {
    auto stream = TcpStream::connect("127.0.0.1:8000");
    BufType buf{};
    std::copy(std::istream_iterator<char>{std::cin}, std::istream_iterator<char>{}, std::back_inserter(buf));
    stream.send(buf);
}