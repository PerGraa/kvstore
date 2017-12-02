cmake_minimum_required(VERSION 3.0)

project(kvstore)

## Unit tests
add_subdirectory(pistache/googletest-release-1.7.0)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
file(GLOB_RECURSE TEST_SOURCES  test/*.cpp test/*.hpp)
add_executable(unittest ${TEST_SOURCES})
target_include_directories(unittest PUBLIC "src")
target_link_libraries(unittest gtest gtest_main)

## Compiler
SET (CMAKE_CXX_COMPILER  "g++-5")

## Pistache
# Let GCC go easy on Pistache
SET (CMAKE_CXX_FLAGS     "-O3 -std=c++14")
add_subdirectory (pistache/src)

## Server
# Let GCC go harder on kvstore itself
SET (CMAKE_CXX_FLAGS     "-O3 -std=c++14   -Wpedantic -Wall -Wextra   -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wno-unused")

file(GLOB_RECURSE SERVER_SOURCES  src/server.cpp src/server/*.[c|h]pp)
add_executable(server ${SERVER_SOURCES})
target_include_directories(server PRIVATE "src/common")
target_link_libraries(server pistache)

## Client
file(GLOB_RECURSE CLIENT_SOURCES  src/client.cpp src/client/*.[c|h]pp)
add_executable(client ${CLIENT_SOURCES})
target_include_directories(client PRIVATE "src/common")
target_link_libraries(client pistache)

## Collect for convenience
file(GLOB_RECURSE KVSTORE_SOURCES ${SERVER_SOURCES} ${CLIENT_SOURCES} src/common/*.[c|h]pp)

## cppcheck
add_custom_target(
  cppcheck
  COMMAND cppcheck
  --enable=all
  --std=c++14
  --language=c++ 
  ${KVSTORE_SOURCES}
  ${TEST_SOURCES}
)

## clang-tidy
add_custom_target(
  clang-tidy
  COMMAND clang-tidy-4.0
#  -fix # Warning: Uncommenting lets clang-tidy perform changes in source
  ${KVSTORE_SOURCES}
  -checks=*,-llvm-header-guard,-header-filter=.* --
  -std=c++14
  -I${PROJECT_SOURCE_DIR}/pistache/include/
  -I${PROJECT_SOURCE_DIR}/src/common/
)

## clang-format
add_custom_target(
  clang-format
  COMMAND clang-format-4.0
  -i
  -style='{ BasedOnStyle: google, 
            Standard: Cpp11, 
            ColumnLimit: 88, 
            AlignConsecutiveAssignments: true,
            SortIncludes: false }' # Conflicts with clang-tidy [llvm-include-order]
  ${KVSTORE_SOURCES}
  ${TEST_SOURCES}
)
