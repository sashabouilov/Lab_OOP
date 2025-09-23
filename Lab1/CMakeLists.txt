cmake_minimum_required(VERSION 3.15)
project(mytest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройки компилятора для GCC
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")
endif()

# Проверка существования исходных файлов
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/scr/closest_pair_tonum.cpp")
    message(FATAL_ERROR "Source file not found: scr/closest_pair_tonum.cpp")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
    message(FATAL_ERROR "Source file not found: main.cpp")
endif()

# Google Test с исправлением deprecated warning
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
    TLS_VERIFY false
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Исправляем deprecated warning
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

# Основная библиотека
add_library(${PROJECT_NAME}_lib scr/closest_pair_tonum.cpp)

# Основное приложение
add_executable(${PROJECT_NAME}_exe main.cpp)
target_link_libraries(${PROJECT_NAME}_exe PRIVATE ${PROJECT_NAME}_lib)

# Тесты (если есть тестовый файл)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/tests01.cpp")
    enable_testing()
    add_executable(tests test/tests01.cpp)
    target_link_libraries(tests PRIVATE ${PROJECT_NAME}_lib gtest_main)
    add_test(NAME ${PROJECT_NAME}_Tests COMMAND tests)
else()
    message(WARNING "Test file not found: test/tests01.cpp")
endif()
