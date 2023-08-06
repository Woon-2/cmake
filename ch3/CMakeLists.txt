cmake_minimum_required(VERSION 3.18)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

if (NOT DEFINED CMAKE_INSTALL_PREFIX)
    message(STATUS "setting CMAKE_INSTALL_PREFIX to
        ${CMAKE_CURRENT_SOURCE_DIR}/install(default install prefix)
        as none was specified."
    )
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install" CACHE FILEPATH
        "Set path to install."
    )
endif()

project(hello_world VERSION 2023.08.01 LANGUAGES CXX)

message(STATUS "Using Generator: ${CMAKE_GENERATOR}")

# [Platform Interface] ///////////////////////////////
add_library(WIN32_INTERFACE INTERFACE)

if (WIN32)
    target_compile_definitions(WIN32_INTERFACE
    INTERFACE
        WIN32_LEAN_AND_MIN NOMINMAX
    )
endif()

# [C++ Interface] ////////////////////////////////////

add_library(CXX_INTERFACE INTERFACE)

# Default compile options for each compiler
if(CMAKE_CXX_COMPILER_ID MATCHES GNU) # https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html
    target_compile_options(CXX_INTERFACE
    INTERFACE
        -fconcepts
        -Wall
        -pedantic
        -Wextra
    )

elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang) # https://clang.llvm.org/docs/ClangCommandLineReference.html
    if(WIN32) # clang-cl
        target_compile_options(CXX_INTERFACE
        INTERFACE
            "/clang:-fcoroutines-ts"
            -fms-compatibility
        )
    else() # AppleClang or Clang on Linux
        target_compile_options(CXX_INTERFACE
        INTERFACE
            -std=c++2a
        )
    endif()

elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC) # https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-160
    target_compile_options(CXX_INTERFACE
    INTERFACE
        /JMC-
        /Zc:preprocessor
        /Zc:__cplusplus
        /Wall
        /wd4668
    )

endif()

# [compilation] //////////////////////////////////////

# make targets

file(GLOB srcs test*.cpp)

message(STATUS "Using sources: ${srcs}")

add_executable(test_suite ${srcs})
add_executable(catchtest catchtest.cpp)

set_target_properties(test_suite
PROPERTIES
    DEBUG_POSTFIX -d
    CXX_STANDARD 20
)

set_target_properties(catchtest
PROPERTIES
    DEBUG_POSTFIX -d
    CXX_STANDARD 20
)

# configure by platforms -----------------------

if(WIN32)
    set_target_properties(CXX_INTERFACE
    PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS OFF
    )
endif()

# ----------------------------- (configure by platforms)

# [link] ////////////////////////////////////

if(WIN32)
    target_link_libraries(test_suite PRIVATE WIN32_INTERFACE)
    target_link_libraries(catchtest PRIVATE WIN32_INTERFACE)
endif()

target_link_libraries(test_suite PRIVATE CXX_INTERFACE)
target_link_libraries(catchtest PRIVATE WIN32_INTERFACE)
target_link_libraries(catchtest PRIVATE Catch2::Catch2)
target_link_libraries(catchtest PRIVATE Catch2::Catch2WithMain)

# [custom target] ///////////////////////////

# Git log presentation
find_package(Git)
if(GIT_FOUND)
    add_custom_target(log
        COMMENT "==========================
        git commit log
        =========================="
        COMMAND ${GIT_EXECUTABLE} log --abbrev-commit --pretty=oneline
    )
endif()

# [install] /////////////////////////////////

install(TARGETS test_suite catchtest
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    INCLUDES DESTINATION ${CMAKE_INSTALL_PREFIX}/inc
)

# [test] ////////////////////////////////////

# Catch2
find_package(Catch2 REQUIRED)

option(BUILD_TESTING "Build test programs" ON)

include(CTest)

if(NOT BUILD_TESTING)
    return()
endif()

enable_testing()

if(WIN32)
    message(STATUS "this is win32")
    add_test(NAME feature_Win32 COMMAND systeminfo)
endif()

add_test(NAME feature_all COMMAND test_suite
    WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
)