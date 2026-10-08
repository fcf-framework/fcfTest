cmake_minimum_required(VERSION 3.0...${CMAKE_VERSION})

project(fcfTestTest)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /MP")
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /Ob1 /GL /LTCG")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
  endif()
  message("MSVC")
else()
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O0 -g")
  else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
  endif()
endif()

add_executable(fcf-test-test 
                  main.cpp
                  tests/case-error.cpp
                  tests/cmd.cpp
                  tests/cmdRun.cpp
                  tests/duration.cpp
                  tests/durationNested.cpp
                  tests/export.cpp
                  tests/fixture.cpp
                  tests/fixture-error.cpp
                  tests/fixture-run.cpp
                  tests/histogram.cpp
                  tests/logger.cpp
                  tests/macro.cpp
                  tests/macro-FCF_TEST_THROW.cpp
                  tests/macro-FCF_TEST_THROW_CHECK.cpp
                  tests/macro-FCF_TEST_DEFINE.cpp
                  tests/order.cpp
                  tests/storage.cpp
                  tests/param.cpp
              )

target_include_directories(fcf-test-test SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)


