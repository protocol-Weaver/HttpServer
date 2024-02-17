cmake_minimum_required(VERSION 3.8.5)
project(HttpServer VERSION 0.1.0 LANGUAGES C CXX)

add_executable(HttpServer server.cpp)
add_library (MyLib HttpServer.cpp)
target_link_libraries(HttpServer MyLib)

