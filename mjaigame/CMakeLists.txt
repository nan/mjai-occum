cmake_minimum_required(VERSION 3.10)
project(mjai_ai CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add dependent subdirectories, telling CMake to use their own source/build directories
# This avoids creating nested build artifact directories under mjaigame/build/
# It assumes these directories exist at the same level as mjaigame.
add_subdirectory(../opponenthoraestimator ../opponenthoraestimator/build)
add_subdirectory(../tenpaiprobestimator ../tenpaiprobestimator/build)
add_subdirectory(../horaestimator ../horaestimator/build)
add_subdirectory(../averagerankcalculator ../averagerankcalculator/build)

# Find necessary packages (Boost is found by bkuc via opponenthoraestimator)
find_package(Threads REQUIRED)

# Source files for the mjaigame executable
set(MJAI_GAME_SOURCES
    main.cpp
    easywsclient.cpp
)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/yuukouhai.cpp)
    list(APPEND MJAI_GAME_SOURCES yuukouhai.cpp)
    message(STATUS "Added mjaigame/yuukouhai.cpp to compilation.")
else()
    message(WARNING "mjaigame/yuukouhai.cpp not found.")
endif()

add_executable(mjai_ai ${MJAI_GAME_SOURCES})

# Include directories will be propagated from the INTERFACE library targets
target_include_directories(mjai_ai PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} # For MJAIGame.hpp, player.h, picojson.h, etc.
)

target_link_libraries(mjai_ai PRIVATE
    averagerankcalculator_lib # INTERFACE
    horaestimator_lib         # STATIC, also brings mjscore etc.
    opponenthoraestimator_lib # INTERFACE, depends on bkuc
    tenpaiprobestimator_lib   # INTERFACE, depends on bkuc
    # bkuc, mjai_util, and Boost are linked transitively via the estimator libs
    Threads::Threads
)

message(STATUS "mjaigame CMakeLists.txt updated to use sibling build directories for dependencies.")
