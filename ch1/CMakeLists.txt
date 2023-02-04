cmake_minimum_required(VERSION 3.18)
project(hello_world VERSION 2023.2.4 LANGUAGES CXX)

enable_language(ASM)

message(STATUS "Env PATH: $ENV{PATH}")
message(STATUS "hello, world!")

find_file(TXT_PATH
    NAMES a.txt
    PATHS ${CMAKE_SOURCE_DIR})
message(STATUS "Found txt: ${TXT_PATH}")

message(STATUS " - ${PROJECT_NAME}")
message(STATUS " - ${PROJECT_SOURCE_DIR}")
message(STATUS " - ${PROJECT_BINARY_DIR}")

message(STATUS " - ${PROJECT_VERSION}")
message(STATUS " - ${PROJECT_VERSION_MAJOR}")
message(STATUS " - ${PROJECT_VERSION_MINOR}")
message(STATUS " - ${PROJECT_VERSION_PATCH}")