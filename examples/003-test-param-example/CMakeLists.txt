cmake_minimum_required(VERSION 3.0...${CMAKE_VERSION})

project(fcfTestParamExample)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

add_executable(test-param-example
                           test-param-example.cpp
                           )

target_include_directories(test-param-example SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
