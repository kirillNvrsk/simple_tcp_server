cmake_minimum_required(VERSION 3.0.0)

project(tcp_server VERSION 0.1
                   DESCRIPTION "Poco echo TCPServer")

set(CMAKE_CXX_STANDARD 17)

find_package(Poco REQUIRED Foundation Net)

if(Poco_FOUND)
    message ("Poco library found.")
else()
    message (FATAL_ERROR "Cannot find Poco library.")
endif()

add_executable(tcp_server main.cpp)
target_link_libraries(tcp_server Poco::Foundation Poco::Net)