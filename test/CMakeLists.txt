cmake_minimum_required(VERSION 3.0...${CMAKE_VERSION})

project(fcfTestTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(fcf-test-test main.cpp)

target_include_directories(fcf-test-test SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)
