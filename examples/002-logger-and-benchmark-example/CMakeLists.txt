cmake_minimum_required(VERSION 3.0...${CMAKE_VERSION})

project(fcfTestLoggerAndBenchmarkExample)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

add_executable(logger-and-benchmark-example
                           logger-and-benchmark-example.cpp
                           )

target_include_directories(logger-and-benchmark-example SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
