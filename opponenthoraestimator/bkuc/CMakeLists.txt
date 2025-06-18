cmake_minimum_required(VERSION 3.10)
project(bkuc CXX)

# Find Boost
find_package(Boost REQUIRED COMPONENTS system program_options thread filesystem) # Add components as needed

# Assuming engine.h and ma_jan.h are header-only implementations for BKUC1681,
# or that their .cpp files would be in this directory.
# If BKUC1681 had a bkuc_engine.cpp, it would be:
# add_library(bkuc STATIC engine.h ma_jan.h bkuc_engine.cpp)
# For now, as an INTERFACE library if it's truly header-only and relies on util.
add_library(bkuc INTERFACE)

target_include_directories(bkuc INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    $<TARGET_PROPERTY:mjai_util,INTERFACE_INCLUDE_DIRECTORIES> # For util.h
)

# Link with Boost if engine.cpp or other bkuc sources need it
target_link_libraries(bkuc INTERFACE Boost::system Boost::program_options Boost::thread Boost::filesystem)

# Add dependency on mjai_util
add_dependencies(bkuc mjai_util)
