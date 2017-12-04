cmake_minimum_required(VERSION 3.0)

project(kvstore)

## Compiler
SET (CMAKE_CXX_COMPILER  "clang++-4.0")

## Standard soft build flags
SET (CMAKE_CXX_FLAGS     "-O3 -std=c++14")

## Unit tests
file(GLOB_RECURSE TEST_SOURCES  test/*.cpp)
add_executable(unittest ${TEST_SOURCES})
target_include_directories(unittest PRIVATE "src")
target_include_directories(unittest PRIVATE "3rdparty")
target_link_libraries(unittest pthread)

## Let Clang go harder on kvstore itself
SET (STRICT_BUILD_FLAGS "-Wpedantic -Wall -Wextra   -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op-parentheses -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wswitch-default -Wno-unused")

## Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package( Boost COMPONENTS system thread REQUIRED )

## Server
file(GLOB_RECURSE SERVER_SOURCES  src/server.cpp src/server/*.[c|h]pp)
add_executable(server ${SERVER_SOURCES})
set_target_properties(server PROPERTIES COMPILE_FLAGS ${STRICT_BUILD_FLAGS})
target_include_directories(server PRIVATE "3rdparty")
target_link_libraries(server ${Boost_LIBRARIES})

## Client
file(GLOB_RECURSE CLIENT_SOURCES  src/client.cpp src/client/*.[c|h]pp)
add_executable(client ${CLIENT_SOURCES})
set_target_properties(client PROPERTIES COMPILE_FLAGS ${STRICT_BUILD_FLAGS})
target_include_directories(client PRIVATE "3rdparty")
target_link_libraries(client pthread)

## Collect for convenience
file(GLOB_RECURSE KVSTORE_SOURCES ${SERVER_SOURCES} ${CLIENT_SOURCES})

## cppcheck
add_custom_target(
  cppcheck
  COMMAND cppcheck
  --enable=all
  --std=c++14
  --language=c++
#  --check-config # Uncomment to see if cppcheck is missing something
  ${KVSTORE_SOURCES}
  ${TEST_SOURCES}
  -I ${PROJECT_SOURCE_DIR}/src/
)

## clang-tidy
add_custom_target(
  clang-tidy
  COMMAND clang-tidy-4.0
#  -fix # Warning: Uncommenting lets clang-tidy perform changes in source
  ${KVSTORE_SOURCES}
  -checks=*,-llvm-header-guard,-header-filter=.* --
  -std=c++14 # Removing this may crash clang-tidy
  -I${PROJECT_SOURCE_DIR}/3rdparty/
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
