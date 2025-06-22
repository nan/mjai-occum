# mjaigame/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(mjai_ai_app CXX) # Changed project name to avoid conflict with top-level
# ... (standard settings, find_package Threads, find_package Boost)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(Boost REQUIRED COMPONENTS system program_options thread filesystem)
find_package(Threads REQUIRED)

set(MJAI_GAME_SOURCES main.cpp easywsclient.cpp)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/yuukouhai.cpp)
    list(APPEND MJAI_GAME_SOURCES yuukouhai.cpp)
endif()

add_executable(mjai_ai ${MJAI_GAME_SOURCES})
target_include_directories(mjai_ai PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(mjai_ai PRIVATE RELEASE_BUILD)
target_link_libraries(mjai_ai PRIVATE
    averagerankcalculator_lib
    horaestimator_lib
    opponenthoraestimator_lib
    tenpaiprobestimator_lib
    # bkuc & mjai_util are linked via opponenthoraestimator_lib
    Threads::Threads
    Boost::system Boost::program_options Boost::thread Boost::filesystem
)
