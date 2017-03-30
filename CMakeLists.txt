cmake_minimum_required(VERSION 3.6)
project(CLION)

set(CMAKE_CXX_STANDARD 11)


add_subdirectory(tests)
set(SOURCE_FILES main.cpp)
include_directories(sampler)
add_executable(CLION ${SOURCE_FILES})

