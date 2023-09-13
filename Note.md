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

## CMake Pattern

- `ranged cmake_minimum_required`: 3.12버전에서 추가된 것으로, `backward compatible`하지만
  3.12버전 이전의 버전에서는 하한 버전에 고정됨, 따라서 `cmake_policy`를 이용해 적절한 버전 적용

```cmake
cmake_minimum_required(VERSION 3.7...3.27)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
```

- `configuration check`: `cmake --build`시 사용할 `configuration`의 기본값 지정

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
check_type_size("((Dragon*)0)->hp" SIZEOF_DRAGON_HP)

message(size of int: ${SIZEOF_INT})
message(size of hp, a member data of the class, Dragon: ${SIZEOF_DRAGON_HP})
```

>Despite the name of the macro you may use it to check the size of more complex expressions, too.

- `HAVE_<variable>`: 해당 타입이 존재하는지를 나타내는 boolean 타입의 변수   
  따라서 타입 크기를 조사할 때에는 굳이 `check_cxx_symbol_exists`를 사용할 필요가 없다.   
  **`HAVE_<type>`이 아닌 `HAVE_<variable>`임에 주의**
