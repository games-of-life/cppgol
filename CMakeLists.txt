cmake_minimum_required(VERSION 3.29)

project(GOL)

set(CMAKE_CXX_STANDARD 20)

find_package(raylib)
find_package(range-v3)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Uncomment when using gcc/clang
# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -pg")
# set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(cppgol ${SOURCES})
target_link_libraries(cppgol raylib range-v3::range-v3)
