# ================================================
# This script behaves as follows
# 0. Check cmake version
# 1. Check build type (If build type is not specified, it sets
#    the build type to 'RelWithDebInfo' as default.)
# 2. Cmake project setup
# 3. C++ setup (prepare default compile options for each compiler.) 
# ------------------------------------------------
# Procedures
# *** 0-3 Project steps
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

### Check cmake version
cmake_minimum_required(VERSION 3.18)

### Check build type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'RelWithDebInfo'(default build type)
        as none was speicifed.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
        "Choose the type of field" FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

### Cmake project setup
project(hello_world VERSION 2023.02.12 LANGUAGES CXX)

### C++ setup
add_library(CXX_DEFAULT_INTERFACE INTERFACE)

# See below link for finding gcc options.
# https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html
if(CMAKE_CXX_COMPILER_ID matches GNU)
    target_compile_options(CXX_DEFAULT_INTERFACE
    INTERFACE
    -fconcepts
    -Wall
    -pedantic
    -Wextra
    )
# See below link for finding clang options.
# https://clang.llvm.org/docs/ClangCommandLineReference.html
elseif(CMAKE_CXX_COMPILER_ID matches CLANG)
    # clang options differ by platform.
    if(WIN32)  # clang-cl
        target_compile_options(CXX_DEFAULT_INTERFACE
        INTERFACE
            "/clang:-fcoroutines-ts"
            -fms-compatibility
        )
    else()  # AppleClang or Clang on linux
        target_compile_options(CXX_DEFAULT_INTERFACE
        INTERFACE
            -std=c++2a
        )
    endif()
# See below link for finding msvc options.
# https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-160
elseif(CMAKE_CXX_COMPILER_ID matches MSVC)
    target_compile_options(CXX_DEFAULT_INTERFACE
    INTERFACE
        /Wall
        /wd4668
        /JMC-
        /Zc:__cplusplus
        /Zc:preprocessor
    )
endif()

# *** Common steps ***

# *** Custom steps ***