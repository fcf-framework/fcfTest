cmake_minimum_required(VERSION 3.0...${CMAKE_VERSION})

project(fcfTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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



add_subdirectory(./examples/full-example)
add_subdirectory(./examples/001-simple-example)
add_subdirectory(./examples/002-logger-and-benchmark-example)
add_subdirectory(./examples/003-logger-function-prefix-example)
add_subdirectory(./test)
