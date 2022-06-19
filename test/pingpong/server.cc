
#include "src/tcplistener.h"

#include <iostream>
#include <iterator>


int main() {
    TcpListener listener("127.0.0.1:8000");


    while (true) {
        auto [ccon, addr] = listener.accept();

        std::cout << "Accept a connection, " << addr.ip().str() << ":" << addr.port();
        std::vector<char> buf(20, 0);
        while (0 != ccon.recv(buf)) {
            std::cout << "Received " << buf.size() << " bytes" << std::endl;
            std::copy(buf.cbegin(), buf.cend(), std::ostream_iterator<char>(std::cout));
            std::cout << std::endl;
        }
    }
}