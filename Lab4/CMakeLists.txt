cmake_minimum_required(VERSION 3.15)
project(TemplateProject VERSION 1.0.0 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()


set(SOURCES
    main.cpp
)


set(HEADERS
    array.h
    point.h
    figure.h
)


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})


target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /Zi)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
    endif()
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /O2)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -O3)
    endif()
endif()


message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")