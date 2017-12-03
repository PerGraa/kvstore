cmake_minimum_required(VERSION 3.0)

project(kvstore)

## Compiler
SET (CMAKE_CXX_COMPILER  "g++-5")

## Standard soft build flags
SET (CMAKE_CXX_FLAGS     "-O3 -std=c++14")

## Unit tests
file(GLOB_RECURSE TEST_SOURCES  test/*.cpp)
add_executable(unittest ${TEST_SOURCES})
target_include_directories(unittest PRIVATE "src")
target_include_directories(unittest PRIVATE "3rdparty")
target_link_libraries(unittest pthread)

## Let GCC go harder on kvstore itself
SET (STRICT_BUILD_FLAGS "-Wpedantic -Wall -Wextra   -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wno-unused")


add_subdirectory (3rdparty/pistache/src)

## Boost
find_package( Boost COMPONENTS system thread REQUIRED )

## Server
file(GLOB_RECURSE SERVER_SOURCES  src/server.cpp src/server/*.[c|h]pp)
add_executable(server ${SERVER_SOURCES})
set_target_properties(server PROPERTIES COMPILE_FLAGS ${STRICT_BUILD_FLAGS})
target_include_directories(server PRIVATE "src/common")
target_include_directories(server PRIVATE "3rdparty")
target_link_libraries(server pistache)
target_link_libraries(server ${Boost_LIBRARIES})

## Client
file(GLOB_RECURSE CLIENT_SOURCES  src/client.cpp src/client/*.[c|h]pp)
add_executable(client ${CLIENT_SOURCES})
set_target_properties(client PROPERTIES COMPILE_FLAGS ${STRICT_BUILD_FLAGS})
target_include_directories(client PRIVATE "src/common" "3rdparty/embeddedRest")
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
  -I${PROJECT_SOURCE_DIR}/3rdparty/pistache/include/
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
