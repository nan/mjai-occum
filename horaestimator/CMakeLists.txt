# horaestimator/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(horaestimator CXX)
# ... (standard settings, find_package Threads)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package(Threads REQUIRED)

add_library(horaestimator_lib STATIC
    mjscore.cpp mjscore.h syanten4hora_estimator.cpp syanten4hora_estimator.h
    MJHoraEstimator.hpp MJRonHoraEstimator.hpp pai.h pai_set.h furo.h util.h bit_vector.h shanten_analysis.h
)
target_include_directories(horaestimator_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Link against opponenthoraestimator_lib for its INTERFACE properties (includes for OpponentHoraEstimator.hpp)
target_link_libraries(horaestimator_lib PUBLIC opponenthoraestimator_lib)

if(BUILD_TEST_EXECUTABLES)
    add_executable(test_horaestimator main.cpp)
    target_link_libraries(test_horaestimator PRIVATE horaestimator_lib Threads::Threads)
endif()
