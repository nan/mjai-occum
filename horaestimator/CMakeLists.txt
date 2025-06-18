# horaestimator/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(horaestimator CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Threads REQUIRED)

add_library(horaestimator_lib STATIC
    mjscore.cpp
    mjscore.h
    syanten4hora_estimator.cpp
    syanten4hora_estimator.h
    MJHoraEstimator.hpp
    MJRonHoraEstimator.hpp
    pai.h
    pai_set.h
    furo.h
    util.h
    bit_vector.h
    shanten_analysis.h
)
target_include_directories(horaestimator_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../opponenthoraestimator # For OpponentHoraEstimator.hpp
    ../opponenthoraestimator/bkuc # For engine.h
    ../opponenthoraestimator/util # For util.h used by bkuc
)
# We are not linking opponenthoraestimator_lib here as this is an independent build.
# The include of OpponentHoraEstimator.hpp means this library uses its interface.
# Full linking integrity will be checked when mjaigame is built.

add_executable(test_horaestimator main.cpp)
target_link_libraries(test_horaestimator PRIVATE horaestimator_lib Threads::Threads)
# For test_horaestimator to link properly if OpponentHoraEstimator.hpp pulls Boost,
# it might need Boost libs here too, or rely on mjaigame's final link.
# For now, focus on compiling horaestimator_lib's own sources.
