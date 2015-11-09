cmake_minimum_required(VERSION 2.8)
project(mld-op)

file(GLOB SRC "*.h*" "*.cpp")

include_directories(../mld-core)

add_library(mld-op STATIC ${SRC})
