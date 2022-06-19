cmake_minimum_required(VERSION 3.23)
project(netlib)

SET(CMAKE_CXX_STANDARD 17)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/lib)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/output/lib)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

add_library(netlib
        src/udpsocket.cc src/tcplistener.cc src/tcplistener.h src/tcpstream.cc src/tcpstream.h src/socket.cc)

add_executable(pingpong_server test/pingpong/server.cc)
add_executable(pingpong_client test/pingpong/client.cc)
target_link_libraries(pingpong_server netlib)
target_link_libraries(pingpong_client netlib)

#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
#)
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
#
#enable_testing()

#add_executable(hello_test src/test/hello_test.cc)
#target_link_libraries(hello_test gtest_main netlib)


#include(GoogleTest)
#gtest_discover_tests(hello_test)
#add_executable(inet_aton_test src/test/inet_aton_test.cc)
#add_executable(split_ipaddr src/test/split_ipaddr.cc)