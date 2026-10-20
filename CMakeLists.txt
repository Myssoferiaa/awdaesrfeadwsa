cmake_minimum_required(VERSION 3.22)

# Load Geode CMake macros
include($ENV{GEODE_SDK}/cmake/GeodeFile.cmake)

project(PredictiveInputBuffer VERSION 4.7.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
)

setup_geode_mod(${PROJECT_NAME})
