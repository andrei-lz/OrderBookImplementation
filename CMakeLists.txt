cmake_minimum_required(VERSION 3.12)
project(BinanceBook)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Set maximum optimization flags for Release build
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -funroll-loops -ffast-math -fno-math-errno -DNDEBUG")


file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(BinanceBook ${SOURCES})
