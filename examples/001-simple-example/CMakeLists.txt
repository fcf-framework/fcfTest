cmake_minimum_required(VERSION 3.0...${CMAKE_VERSION})

project(fcfTestSimpleExample)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(simple-example
                           simple-example.cpp
                           )

target_include_directories(simple-example SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
