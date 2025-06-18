cmake_minimum_required(VERSION 3.10)
project(mjai_util CXX)

# Assuming util.h is header-only with static methods,
# we create an INTERFACE library to manage include directories.
# If there was a util.cpp, we'd use add_library(mjai_util STATIC util.cpp util.h)
add_library(mjai_util INTERFACE)
target_include_directories(mjai_util INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
