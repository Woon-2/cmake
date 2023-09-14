# CMake Note

## CMake Syntax

- 콤마 `,` 대신 공백 사용
- 변수는 문자열, `변수`라는 변수를 참조하려면 `${변수}`와 같이 사용
- 큰따옴표로 감싸진 문자열과 감싸지지 않은 문자열을 구분하지 않음
- 변수는 Case Sensitive, 키워드나 명령어는 Case Insensitive

## Common Semantics

- 변수는 무조건 대문자, 함수나 매크로는 소문자로 작성 (키워드도 결국 함수의 인자이기 때문에 대문자)
- 경로와 같이 공백이 중간에 들어갈 가능성이 높은 문자열은 무조건 큰따옴표로 감싸기   
  ex) `set(WIN32_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Win32" CACHE STRING "Win32 Path")`
- `PRIVATE`/`PUBLIC`/`INTERFACE`의 의미를 이해하기
  
  >- using a dependency in the implementation of a library, but not in the header files (PRIVATE)    
  >- using a dependency in the implementation of a library and also in the header files (PUBLIC)    
  >- using a dependency in the header files of a library but not in the implementation (INTERFACE)

## CMake Variables

- `CMAKE_BUILD_TYPE`: 지정되지 않았을 경우 기본 빌드 타입으로 빌드하도록 하기
- `CMAKE_CONFIGURATION_TYPES`: 다양한 빌드 모드를 지원하는 빌드 시스템을 사용하는 경우(msvc) 지정,   
  리스트 변수임을 유의하기!
- `BUILD_SHARED_LIBS`: `add_library`에 `STATIC`/`SHARED`를 명시적으로 지정하지 않으면 이 변수의 값을 따름! 체크하기
- `BUILD_TESTING`: `BUILD_TESTING`이 활성화되어있을 경우 `add_subdirectory`로 테스트 디렉터리의 cmake 스크립트가 실행되도록
- `CMAKE_MODULE_PATH`: `.cmake` 스크립트를 찾을 경로 모음, `semicolon-seperated list`, 리스트에 스크립트들이 있는 디렉터리를 추가하기
- `CMAKE_SOURCE_DIR`: 소스 트리의 최상위 디렉터리
- `CMAKE_BINARY_DIR`: 빌드 트리의 최상위 디렉터리
- `CMAKE_CURRENT_LIST_DIR`: 현재 처리되고 있는 cmake 스크립트가 있는 디렉터리
- `CMAKE_CURRENT_SOURCE_DIR`: `add_subdirectory`를 할 때마다 갱신되는, 현재 소스 디렉터리
- `CMAKE_CURRENT_BINARY_DIR`: `add_subdirectory`를 할 때마다 갱신되는, 현재 빌드 디렉터리
- `PROJECT_SOURCE_DIR`: 가장 최근의 `project` 명령어가 실행된 소스 디렉터리    
  `add_subdirectory` 등으로 `project`가 중첩될 때마다 갱신되고,    
  중첩된 스크립트를 처리하고 난 뒤에는 원래대로 복구된다.
- `PROJECT_BINARY_DIR`: 가장 최근의 `project` 명령어가 실행된 빌드 디렉터리    
  `add_subdirectory` 등으로 `project`가 중첩될 때마다 갱신되고,    
  중첩된 스크립트를 처리하고 난 뒤에는 원래대로 복구된다.
- `CMAKE_INSTALL_PREFIX`: 인스톨 경로, 인스톨 시 이 경로의 하위 디렉터리들로 빌드 파일들을 위치시키면 편리하다.

## Target Construction

`target`을 선언한 뒤에는, 다음 순서로 `target`을 구축하는 것이 보기 좋다.

1. `target_sources`
2. `target_compile_features`
3. `target_compile_definitions`
4. `target_compile_options`
5. `target_link_libraries`
6. `target_link_options`
7. `target_include_directories`
8. `target_precompile_headers`

- 모든 명령어가 `target_...s`의 형태이고 첫번째 인자로 `target`을 받는 것을 명심하자.
- 이 명령어들은 내부적으로 `target`의 `property`들을 조작한다.    
  직접 `target`의 `property`를 조작하는 일은 최소화하자.

### target_sources

`target`에 소스를 추가한다.

```cmake
target_sources(<target>
    <INTERFACE|PUBLIC|PRIVATE> [items1...]
    [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

- 소스 파일의 경로를 절대 경로로 지정하지 않을 경우 `CMAKE_CURRENT_SOURCE_DIR`에 대한 상대 경로로 인식된다.

  - generator expression 안에선 이 규칙이 적용되지 않으므로, generator expression과 함께 쓰일 때에는    
    `$<$<CONFIG:DEBUG>:${CMAKE_CURRENT_SOURCE_DIR}/source_debug.cpp`처럼 절대 경로로 작성한다.

- `target_include_directories`는 필요한 헤더 파일들을 include해주지 않는다.    
  `target_sources`에 필요한 헤더들을 명시해야한다.

- 3.23버전에서 `FILE_SET`이 추가, TBA

### target_compile_features

요구되는 C++ 표준 버전이 있을 때 사용

```cmake
add_library(mylib requires_cxx20.cpp)
# C++20 is a usage-requirement
target_compile_features(mylib PUBLIC cxx_std_20)
```

### target_compile_definitions

조건부 컴파일문 등에 사용할 전처리기를 cmake단에서 정의    
그 외에 cmake 변수를 활용하는 전처리기들은 `target_compile_definition`이 아닌 `configure_file`을 통해 작성한다.

```cmake
target_compile_definitions(<target>
    <INTERFACE|PUBLIC|PRIVATE> [items1...]
    [<INTERFACE|PUBLIC|PRIVATE> [items2...]...])

target_compile_definitions(my_target PRIVATE $<$<CONFIG:Release>:NDEBUG>)
```

### target_compile_options

각 컴파일러의 컴파일 옵션들을 참고하여 작성한다.    
[cmake가 지원하는 컴파일러 리스트](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_ID.html)

- [`gcc`/`g++` 컴파일 옵션](https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html)
- [`msvc` 컴파일 옵션](https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-160)
- [`clang` 컴파일 옵션](https://clang.llvm.org/docs/ClangCommandLineReference.html)

```cmake
target_compile_options(<target> [BEFORE]
    <INTERFACE|PUBLIC|PRIVATE> [items1...]
    [<INTERFACE|PUBLIC|PRIVATE> [items2...]...])

if(CMAKE_<LANG>_COMPILER_ID MATCHES <COMPILER_ID>)
    target_compile_options(...)
[elseif(CMAKE_<LANG>_COMPILER_ID MATCHES <ADDITIONAL_COMPILER_ID>...]
endif()
```

### target_link_libraries

라이브러리(`.dll`/`.lib` 등)의 절대 경로나 스크립트 내에서 식별되는 타겟 이름을 사용하여    
외부 라이브러리를 타겟에 링크한다.

```cmake
target_link_libraries(<target>
    <PRIVATE|PUBLIC|INTERFACE> <item>...
    [<PRIVATE|PUBLIC|INTERFACE> <item>...]...)


set(LIBNAME $<IF:$<CONFIG:Debug>:woon-debug.lib,woon.lib> CACHE)

target_link_libraries(
    mylib
    PUBLIC externlib::externlib innerlib
    PRIVATE $<BUILD_INTERFACE:${CMAKE_PROJECT_BINARY_DIR}/lib/${LIBNAME}> $<INSTALL_INTERFACE:lib/${LIBNAME}>
)
```

### target_link_options

3.13버전에서 추가된 것임에 주의.
각 링커의 링크 옵션을 참고하여 작성한다.

```cmake
target_link_options(<target> [BEFORE]
    <INTERFACE|PUBLIC|PRIVATE> [items1...]
    [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])

target_link_options(Win32 BEFORE PRIVATE
    /SUBSYSTEM:WINDOWS /ENTRY:WinMainCRTStartup
)
```

### target_include_directories

빌드에 필요한 헤더 파일들이 있는 디렉터리를 지정한다.    
이 디렉터리는 `#include`문을 만날 때 찾을 경로가된다.    
따라서 헤더들을 include할 때 소스 파일에 대해 상대 경로로 지정할 필요가 없어진다.    
ex) `#include "../include/header.hpp"` -> `#include "header.hpp"`

```cmake
target_include_directories(<target> [SYSTEM] [AFTER|BEFORE]
    <INTERFACE|PUBLIC|PRIVATE> [items1...]
    [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])

target_include_directories(
    mylib
    PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>
)
```

- `SYSTEM` 인자는 외부 라이브러리의 경고를 무시하는 용도로도 쓸 수 있다.

### target_precompile_headers

TBA

## CMake Pattern

- `ranged cmake_minimum_required`: 3.12버전에서 추가된 것으로, `backward compatible`하지만    
  3.12버전 이전의 버전에서는 하한 버전에 고정됨, 따라서 `cmake_policy`를 이용해 적절한 버전 적용

```cmake
cmake_minimum_required(VERSION 3.7...3.27)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
```

- `default configuration`: `cmake --build`시 사용할 `configuration`의 기본값 지정

```cmake
# Single Configuration Build System의 경우

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(DEFAULT_BUILD_TYPE Release)

    message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE
        STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
```

```cmake
# Multi-Configuration Build System의 경우

if(NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(DEFAULT_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo"

    message(STATUS "Setting configuration types to '${DEFAULT_CONFIGURATION_TYPES}' as none was specified.")
    set(CMAKE_CONFIGURATION_TYPES "${DEFAULT_CONFIGURATION_TYPES}" CACHE
        STRING "Write the possible configuration types." FORCE)
endif()
```

- `default install prefix`: 인스톨 경로의 기본값 지정

```cmake
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    set(DEFAULT_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}"/install)

    message(STATUS "Setting install prefix to '${DEFAULT_INSTALL_PREFIX}' as none was specified.")
    set(CMAKE_INSTALL_PREFIX "${DEFAULT_INSTALL_PREFIX}" CACHE
        STRING "Set the install path")
endif()
```

- `project configuration`: 프로젝트의 이름과 메타데이터 및 **타겟 플랫폼과 언어 관련 정보 설정(툴체인 기반)**

> - If CMAKE_TOOLCHAIN_FILE is set, read it at least once. It may be read multiple times and it may also be read again when enabling languages later (see below).
> - Set the variables describing the host and target platforms. Language-specific variables might or might not be set at this point. On the first run, the only language-specific variables that might be defined are those a toolchain file may have set. On subsequent runs, language-specific variables cached from a previous run may be set.

```cmake
project(<PROJECT-NAME>
        [VERSION <major>[.<minor>[.<patch>[.<tweak>]]]]
        [DESCRIPTION <project-description-string>]
        [HOMEPAGE_URL <url-string>]
        [LANGUAGES <language-name>...])
```

- `resource paths management`: 환경에 따라서 달라질 수 있는 리소스 경로들을 cmake가 제공,
  `.in`으로 끝나는 템플릿을 cmake가 처리하여 실제 헤더파일을 생산한다.

##### resource.h.in:

```cpp
#ifndef __resources
#define __resources

#define PATH_IMAGES "@IMAGE_RESOURCE_PATH@"    // identifiers within @ are replaced by cmake
#define PATH_SOUNDS "@SOUND_RESOURCE_PATH@"    // via "configure_file" command

#endif
```

##### CMake lines:

```cmake
set(IMAGE_RESOURCE_PATH "${PROJECT_SOURCE_DIR}/resource/image")
set(SOUND_RESOURCE_PATH "${PROJECT_SOURCE_DIR}/resource/sound")

configure_file(
    "${PROJECT_SOURCE_DIR}/include/resources.h.in"
    "${PROJECT_BINARY_DIR}/include/resources.h"
)
```

- `install interface & build interface`: 빌드 시와 인스톨 시에 다른 값을 가지는 변수 생성

```cmake
target_include_directories(
    MyTarget
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
```

```cmake
set(
    IMAGE_RESOURCE_PATH
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/resource/image>
    $<INSTALL_INTERFFACE:resource/image>
    CACHE
)
mark_as_advanced(IMAGE_RESOURCE_PATH)
```

[`generator expression` 안에서는 문자열을 quote하지 말 것](https://gitlab.kitware.com/cmake/cmake/-/issues/19593)

- `stand-alone check': stand-alone으로 빌드할 시와 다른 프로젝트의 하위 프로젝트로 빌드할 시의 설정을 분리

```cmake
# cmake 버전 3.21 이상에서
if(PROJECT_IS_TOP_LEVEL)
    do_something(...)
endif()

# cmake 버전 3.21 미만에서
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    do_something(...)
endif()
```

## Useful Modules

### TestBigEndian

```cmake
test_big_endian(<result-variable>)

test_big_endian(RES)
message(STATUS "Is target system big endian: ${RES})
```

### CheckSymbolExists, CheckCXXSymbolExists

```cmake
check_cxx_symbol_exists(<symbol> <headers of semicolon-separated list> <result-variable>)

check_cxx_symbol_exists(std::int32_t cstdint RES)
message(STATUS "Is int32_t exists: ${RES})
```

- `CMAKE_REQUIRED_...` 변수들을 조작해야 하는데, 커스텀으로 `try_compile`과 `try_run` 비슷한 함수를 만들면   
  `CMAKE_REQUIRED_...`를 직접 조작하지 않을 수 있다.   
- 물론, `cmake_push_state`, `cmake_reset_state`를 사용하는 방법도 있다.

### CheckTypeSize

```cmake
check_type_size(<type> <variable> [BUILTIN_TYPES_ONLY] [LANGUAGE <language>])

check_type_size(int SIZEOF_INT LANGUAGE CXX)

message(size of int: ${SIZEOF_INT})
```

>Despite the name of the macro you may use it to check the size of more complex expressions, too.

- `HAVE_<variable>`: 해당 타입이 존재하는지를 나타내는 boolean 타입의 변수   
  따라서 타입 크기를 조사할 때에는 굳이 `check_cxx_symbol_exists`를 사용할 필요가 없다.   
  **`HAVE_<type>`이 아닌 `HAVE_<variable>`임에 주의**
