cmake_minimum_required(VERSION 3.16)
project(sw_battle_test)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/**/*.cpp")

add_executable(sw_battle_test ${SOURCES})

target_include_directories(sw_battle_test PRIVATE "${CMAKE_SOURCE_DIR}/src")
