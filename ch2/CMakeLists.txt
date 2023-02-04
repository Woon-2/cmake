cmake_minimum_required(VERSION 3.18)
project(hello_world VERSION 2023.02.04 LANGUAGES CXX)

message(STATUS "Using Generator: ${CMAKE_GENERATOR}")

# Compilation

add_library(A)
target_sources(A PRIVATE src/test.cpp inc/main.h inc/test.h)

add_executable(main src/main.cpp)

# add_executable(main src/main.cpp src/test.cpp)

target_include_directories(main
PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/public>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source> # for which purpose?
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
PRIVATE
    inc # relative to ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(main
PROPERTIES
    DEBUG_POSTFIX -d
    CXX_STANDARD 20
)

if(WIN32)
    message(STATUS "Win32 Processing")
    set_target_properties(main
    PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS OFF
    )
    target_compile_definitions(main
    PRIVATE
        WIN32_LEAN_AND_MEAN NOMINMAX
    )
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES GNU) # https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html
    target_compile_options(main
    PRIVATE
        -fconcepts
    )

elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang) # https://clang.llvm.org/docs/ClangCommandLineReference.html
    if(WIN32) # clang-cl
        target_compile_options(main
        PRIVATE
            "/clang:-fcoroutines-ts"
            -fms-compatibility
        )
    else() # AppleClang or Clang on Linux
        target_compile_options(main
        PRIVATE
            -std=c++2a
        )
    endif()

elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC) # https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-160
    target_compile_options(main
    PRIVATE
        /JMC-
        /Zc:preprocessor
    )

endif()

# Link

target_link_libraries(main
PRIVATE
    A
)

if(WIN32)
    target_link_libraries(main
    PUBLIC
        Kernel32
        WindowsApp
    )

elseif(APPLE) # expect AppleClang
    target_compile_options(main
    PUBLIC
        -stdlib=libc++
    )
    target_link_libraries(main
    PUBLIC
        c++
    )

endif()

if(MSVC) # https://docs.microsoft.com/en-us/cpp/build/reference/linker-options?view=msvc-160
    target_link_options(main
    PRIVATE
        /ERRORREPORT:SEND
    )
endif()

# Build

find_package(Git)
if(GIT_FOUND)
    # Build와 Push를 합칠 수 있겠는데?
    add_custom_target(log
        COMMENT "**** git commit log ****"
        COMMAND ${GIT_EXECUTABLE} --version
        COMMAND ${GIT_EXECUTABLE} log --abbrev-commit --pretty=oneline
        COMMAND ${GIT_EXECUTABLE} log --abbrev-commit --stat -1
    )
    add_custom_target(log_detailed
        COMMAND ${GIT_EXECUTABLE} log --abbrev-commit -p -1
    )
    add_dependencies(log_detailed log)
endif()

#install

install(FILES   LICENSE
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}
)

install(TARGETS main
        RUNTIME  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
        LIBRARY  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        INCLUDES DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)