cmake_minimum_required(VERSION 3.10)
project(averagerankcalculator CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the library
# Assuming AverageRankCalculator.hpp is header-only,
# we'll create an INTERFACE library.
# If there was an AverageRankCalculator.cpp, it would be:
# add_library(averagerankcalculator_lib STATIC AverageRankCalculator.cpp AverageRankCalculator.hpp)
# target_include_directories(averagerankcalculator_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# For a header-only library:
add_library(averagerankcalculator_lib INTERFACE)
target_include_directories(averagerankcalculator_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Add an executable for testing
add_executable(test_averagerankcalculator main.cpp)
target_link_libraries(test_averagerankcalculator PRIVATE averagerankcalculator_lib)

message(STATUS "AverageRankCalculator CMakeLists.txt created.")
