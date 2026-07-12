cmake_minimum_required(VERSION 3.0...${CMAKE_VERSION})

project(fcfTestTest)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

add_executable(fcf-test-test 
                  main.cpp
                  tests/cmd.cpp
                  tests/export.cpp
                  tests/macro.cpp
              )

target_include_directories(fcf-test-test SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)
