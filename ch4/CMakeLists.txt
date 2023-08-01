# ************************************************
# This cmake script describes how to build the project.
# Building process in here is the top level of the build tree,
# so this script participates in the entire building process.
#
# /// [For Users] ///
#   type following commands on CLI to make your sources
#   into an executable, or a library.
#   1. cd [] (type the directory this file is located in [].)
#   2. cmake -S. -Bbuild
#   3. cmake --build build
# /////////////////// 
#
# ************************************************
# ================================================
# This script behaves as follows
# 0. Check cmake version
# 1. Check build type (If build type is not specified, it sets
#    the build type to 'RelWithDebInfo' as default.)
# 2. Cmake project setup
# 3. C++ setup (Prepare default compile options for each compiler.) 
# 4. Platform setup (Shared settings for targets on each platform)
# 5. Module test (Just experiments for studying)
# 6. Make executable target
# 7. Git log command (A custom target to provide git log during building)
# ------------------------------------------------
# Procedures
# *** 0-4 Project steps
#     Setup operations before creating any concrete target
#     which includes at least one of header or source.
#     Default language & compile & link settings
#     which is used by all of targets or even other project.
# *** 5-6 Common steps
#     Operations which has to be executed on every single build.
# *** 7-7 Custom steps
#     Optional operations executed when given additional cmake
#     command: via custom target, custom command, etc.
# ------------------------------------------------
# ================================================

#################
# Project steps #
#################

# *** 0. Check cmake version ***

cmake_minimum_required(VERSION 3.18)


# *** 1. Check build type ***
# If build type is not specified, it sets the build type
# to 'RelWithDebInfo' as default.

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'RelWithDebInfo'(default build type)
        as none was speicifed.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
        "Choose the type of field" FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()


# *** 2. Cmake project setup ***

project(hello_world VERSION 2023.08.01 LANGUAGES CXX)


# *** 3. C++ setup ***
# Prepare default compile options for each compiler.

add_library(CXX_DEFAULT_INTERFACE INTERFACE)

# See below link for finding gcc options.
# https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    target_compile_options(CXX_DEFAULT_INTERFACE
    INTERFACE
    -fconcepts
    -Wall
    -pedantic
    -Wextra
    )

# See below link for finding clang options.
# https://clang.llvm.org/docs/ClangCommandLineReference.html
elseif(CMAKE_CXX_COMPILER_ID MATCHES CLANG)
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
elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    target_compile_options(CXX_DEFAULT_INTERFACE
    INTERFACE
        /Wall
        /wd4668
        /wd4464
        /wd4514
        /wd5045
        /JMC-
        /Zc:__cplusplus
        /Zc:preprocessor
    )
endif()


# *** 4. Platform setup ***
# Shared settings for targets on each platform

# WIN32
add_library(WIN32_DEFAULT_INTERFACE INTERFACE)

target_compile_definitions(WIN32_DEFAULT_INTERFACE
INTERFACE
    WIN32_LEAN_AND_MIN NOMINMAX
)

set_target_properties(WIN32_DEFAULT_INTERFACE
PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
)

# Other platforms' setup is to be updated.


################
# Common steps #
################

# *** 5. Testing modules ***
# Just experiments for studying

include(CheckCXXSymbolExists)

check_symbol_exists(SEEK_SET "cstdio" have_seek_set)

find_package(OpenGL)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)

message(STATUS "OpenGL Found: ${OPENGL_FOUND}")
message(STATUS "OpenGL Library Path: ${OPENGL_LIBRARIES}")

function(fn VAR_NAME)
    set(${VAR_NAME} "hello?" PARENT_SCOPE)
endfunction()

fn(hell)
message(STATUS "${hell}")

# *** 6. Make executable target ***

# Prepare dependencies first

macro(CREATE_INTERFACE_LIBRARY libName srcs)
    add_library(${libName} INTERFACE)
    target_sources(${libName} INTERFACE ${srcs})

    target_link_libraries(${libName}
    INTERFACE
        CXX_DEFAULT_INTERFACE
        WIN32_DEFAULT_INTERFACE
    )
endmacro()

# - Make Wrapper library
CREATE_INTERFACE_LIBRARY(Wrapper "inc/wrapper.h")

# - Make Onehot library
CREATE_INTERFACE_LIBRARY(Onehot "inc/onehot.h")

# Then make executable with dependencies
add_executable(Main)
target_sources(Main PRIVATE src/main.cpp)

set_target_properties(Main
PROPERTIES
    DEBUG_POSTFIX -d
    CXX_STANDARD 20
)

target_link_libraries(Main
PRIVATE
    Wrapper Onehot
)

################
# Custom steps #
################

# *** 7. Git log command ***
# A custom target to provide git log during building

find_package(Git)
if(GIT_FOUND)
    add_custom_target(log
        COMMENT "Git Commit Log:"
        COMMAND ${GIT_EXECUTABLE} log --pretty=reference -6
    )
endif()

# TODO: PkgConfig 모듈 이용해서 라이브러리 찾고 링크해보기