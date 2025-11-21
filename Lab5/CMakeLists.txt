cmake_minimum_required(VERSION 3.15)
project(PMRStackProject VERSION 1.0.0 LANGUAGES CXX DESCRIPTION "Custom Stack with PMR Memory Resource")

cmake_policy(SET CMP0077 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive-")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
endif()

add_executable(pmr_stack_demo main.cpp)

target_compile_features(pmr_stack_demo PRIVATE cxx_std_17)
target_include_directories(pmr_stack_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(MINGW)
    target_link_libraries(pmr_stack_demo PRIVATE stdc++fs)
endif()

if(WIN32)
    target_compile_definitions(pmr_stack_demo PRIVATE 
        _CRT_SECURE_NO_WARNINGS
        _SCL_SECURE_NO_WARNINGS
    )
endif()

message(STATUS "PMR Stack Project Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Compiler version: ${CMAKE_CXX_COMPILER_VERSION}")