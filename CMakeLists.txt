cmake_minimum_required(VERSION 3.12)

project(
    itmoscript_interpreter
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_BUILD)
    if(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4")
    else()
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g")
    endif()
else()
    if(MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    endif()
endif()

# Output the build type
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

include_directories(lib)
include_directories(build)
add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)