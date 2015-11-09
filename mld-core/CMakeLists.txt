cmake_minimum_required(VERSION 2.8)
project(mld-core)

file(GLOB SRC "*.h*" "*.cpp")

add_library(mld-core STATIC ${SRC})
