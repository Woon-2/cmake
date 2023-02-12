# ================================================
# This script behaves as follows
# 0. Check cmake version
# 1. Check build type (If build type is not specified, it sets
#    the build type to 'RelWithDebInfo' as default.)
# 2. Cmake project setup
# 
# ------------------------------------------------
# Procedures
# *** 0-2 Project steps
#     Setup operations before creating any concrete target
#     which includes at least one of header or source,
#     Default language & compile & link settings
#     which is used by all of targets or even other project.
# *** Common steps
#     Operations which has to be executed on every single build.
# *** Custom steps
#     Optional operations executed when given additional cmake
#     command: via custom target, custom command, etc.
# ================================================

# *** Project steps ***

# Check cmake version
cmake_minimum_required(VERSION 3.18)

# Check build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'RelWithDebInfo'(default build type)
        as none was speicifed.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
        "Choose the type of field" FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

# Cmake project setup
project(hello_world VERSION 2023.02.12 LANGUAGES CXX)

# *** Common steps ***

# *** Custom steps ***