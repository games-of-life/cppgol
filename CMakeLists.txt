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
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(cppgol ${SOURCES})
target_link_libraries(cppgol raylib)
