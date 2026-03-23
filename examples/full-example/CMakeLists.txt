cmake_minimum_required(VERSION 3.0...${CMAKE_VERSION})
project(fcfTestFullExample)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/../../../)

add_executable("full-example"
                           full-example.cpp
                           )

