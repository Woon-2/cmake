cmake_minimum_required(VERSION 3.18)

if(NOT CMAKE_BUILD_TYPE and NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'RelWithDebInfo'(default build type) as none was speicifed.")
endif()

project(hello_world VERSION 2023.02.12 LANGUAGES CXX)