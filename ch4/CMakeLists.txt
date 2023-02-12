# ================================================
# This script behaves as follows
# 1. Check build type (If build type is not specified, it sets
#    the build type to 'RelWithDebInfo' as default.)
# 2. Cmake project setup
# ================================================

cmake_minimum_required(VERSION 3.18)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'RelWithDebInfo'(default build type)
        as none was speicifed.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
        "Choose the type of field" FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

project(hello_world VERSION 2023.02.12 LANGUAGES CXX)