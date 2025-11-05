cmake_minimum_required(VERSION 3.10)
project(FiguresProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(figures_main
    main.cpp
    figure.cpp
    rhombus.cpp
    pentagon.cpp
    hexagon.cpp
)

target_include_directories(figures_main PRIVATE src)

find_package(GTest QUIET)

if(GTEST_FOUND)
    message(STATUS "GoogleTest found, building tests")
    
    add_executable(figures_test
        test_figures.cpp
        figure.cpp
        rhombus.cpp
        pentagon.cpp
        hexagon.cpp
    )

    target_include_directories(figures_test PRIVATE src)
    target_link_libraries(figures_test GTest::gtest GTest::gtest_main)

    enable_testing()
    add_test(NAME FiguresTest COMMAND figures_test)
else()
    message(WARNING "GoogleTest not found, skipping tests")
endif()