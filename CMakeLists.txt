cmake_minimum_required(VERSION 3.26)
project(untitled6)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled6 main.cpp)


find_package(glad CONFIG REQUIRED)
target_link_libraries(untitled6 PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(untitled6 PRIVATE glfw)

