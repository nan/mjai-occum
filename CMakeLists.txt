cmake_minimum_required(VERSION 3.10)
project(MahjongAI_Suite CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Options to control build of test executables (can be turned OFF to speed up library-only builds)
option(BUILD_TEST_EXECUTABLES "Build test executables for each subproject" ON)

# Add subdirectories for each project.
# CMake will process their CMakeLists.txt files.
# It's important that these sub-project CMakeLists.txt files are
# configured to build libraries and optionally test executables,
# and that they don't add_subdirectory to each other in conflicting ways.

add_subdirectory(averagerankcalculator)
add_subdirectory(opponenthoraestimator) # This directory contains util and bkuc as further subdirectories
add_subdirectory(horaestimator)
add_subdirectory(tenpaiprobestimator)
add_subdirectory(mjaigame)

message(STATUS "Top-level CMakeLists.txt processed.")
message(STATUS "To build, create a build directory, cd into it, run 'cmake ..', then 'make'.")
message(STATUS "For example: mkdir build; cd build; cmake .. -DBUILD_TEST_EXECUTABLES=ON; make")

# Note on potential issues:
# The bkuc engine (inside opponenthoraestimator) loads data files using relative paths
# like "source/..." and "ShantenTableNew.txt".
# The main executable 'mjai_ai' (from mjaigame) will need to be run from a directory
# where these files are accessible, typically meaning the 'opponenthoraestimator/exeと同じ階層に置くファイル/'
# content should be copied to the runtime directory of 'mjai_ai'.
