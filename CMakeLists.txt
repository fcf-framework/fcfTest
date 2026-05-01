cmake_minimum_required(VERSION 3.0...${CMAKE_VERSION})

project(fcfTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(./examples/full-example)
add_subdirectory(./examples/001-simple-example)
add_subdirectory(./examples/002-logger-and-benchmark-example)
add_subdirectory(./test)
