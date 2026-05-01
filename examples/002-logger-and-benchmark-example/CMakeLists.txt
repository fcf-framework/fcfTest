cmake_minimum_required(VERSION 3.0...${CMAKE_VERSION})

project(fcfTestLoggerAndBenchmarkExample)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(logger-and-benchmark-example
                           logger-and-benchmark-example.cpp
                           )

target_include_directories(logger-and-benchmark-example SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../..)