# tenpaiprobestimator/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(tenpaiprobestimator CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(tenpaiprobestimator_lib INTERFACE)
target_include_directories(tenpaiprobestimator_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../opponenthoraestimator/bkuc
    ../opponenthoraestimator/util
)
# Again, not linking bkuc here for independent lib build.
# It uses bkuc headers.

add_executable(test_tenpaiprobestimator main.cpp)
target_link_libraries(test_tenpaiprobestimator PRIVATE tenpaiprobestimator_lib)
