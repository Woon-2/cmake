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
- `BUILD_TESTING`: BUILD_TESTING이 활성화되어있을 경우 `add_subdirectory`로 테스트 디렉터리의 cmake 스크립트가 실행되도록
