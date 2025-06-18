cmake_minimum_required(VERSION 3.10)
project(opponenthoraestimator CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Include subdirectories for util and bkuc libraries
add_subdirectory(util)
add_subdirectory(bkuc)

# Find Boost (bkuc depends on it)
find_package(Boost REQUIRED COMPONENTS system program_options thread filesystem) # Add more components if needed by bkuc

# Add the opponenthoraestimator library
# Assuming OpponentHoraEstimator.hpp is header-only.
# If OpponentHoraEstimator.cpp existed:
# add_library(opponenthoraestimator_lib STATIC OpponentHoraEstimator.cpp OpponentHoraEstimator.hpp)
add_library(opponenthoraestimator_lib INTERFACE)
target_include_directories(opponenthoraestimator_lib INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    $<TARGET_PROPERTY:bkuc,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:mjai_util,INTERFACE_INCLUDE_DIRECTORIES>
)
# Link with bkuc (which links with mjai_util and Boost)
target_link_libraries(opponenthoraestimator_lib INTERFACE bkuc)

# Add an executable for testing
add_executable(test_opponenthoraestimator main.cpp)
target_link_libraries(test_opponenthoraestimator PRIVATE opponenthoraestimator_lib)
# Explicitly link Boost to the executable if main.cpp uses Boost directly,
# or if INTERFACE libraries don't correctly propagate Boost::boost usage.
# bkuc should propagate its Boost dependency.

message(STATUS "OpponentHoraEstimator CMakeLists.txt updated.")
