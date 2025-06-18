# opponenthoraestimator/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(opponenthoraestimator CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # Good for libraries

add_subdirectory(util) # Defines mjai_util
add_subdirectory(bkuc) # Defines bkuc

find_package(Boost REQUIRED COMPONENTS system program_options thread filesystem)

add_library(opponenthoraestimator_lib INTERFACE)
target_include_directories(opponenthoraestimator_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(opponenthoraestimator_lib INTERFACE bkuc) # bkuc brings mjai_util and Boost

if(BUILD_TEST_EXECUTABLES)
    add_executable(test_opponenthoraestimator main.cpp)
    target_link_libraries(test_opponenthoraestimator PRIVATE opponenthoraestimator_lib)
endif()
