cmake_minimum_required(VERSION 3.0...${CMAKE_VERSION})

project(fcfTestFullExample)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(full-example
                           full-example.cpp
                           )

target_include_directories(full-example SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
