cmake_minimum_required(VERSION 3.14)

project(GOL)

include(FetchContent)

set(FETCHCONTENT_QUIET false)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Uncomment when using gcc/clang
# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -pg")
# set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(${PROJECT_SOURCE_DIR}/include)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "4.5.0"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
    range-v3
    GIT_REPOSITORY "https://github.com/ericniebler/range-v3.git"
    GET_TAG "v0.12.0"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(range-v3)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(cppgol ${SOURCES})
target_link_libraries(cppgol raylib range-v3)
