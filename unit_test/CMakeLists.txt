cmake_minimum_required(VERSION 2.8)
project(unit_test)

file(GLOB SRC "*.h*" "*.cpp")

include_directories(../gtest-1.7.0/include ../mld-core)

add_executable(unit_test ${SRC})

target_link_libraries(unit_test mld-op mld-core gtest gtest_main dbghelp.lib)
