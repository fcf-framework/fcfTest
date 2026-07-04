cmake_minimum_required(VERSION 3.0...${CMAKE_VERSION})

project(fcfTestLoggerFunctionPrefixExample)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(logger-function-prefix-example
                           logger-function-prefix-example.cpp
                           )

target_include_directories(logger-function-prefix-example SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
