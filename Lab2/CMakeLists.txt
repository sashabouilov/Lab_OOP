cmake_minimum_required(VERSION 3.14)
project(SixNumber VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    if(CMAKE_GENERATOR STREQUAL "NMake Makefiles")
        message(WARNING "NMake may not work well with Google Test. Consider using Visual Studio or MinGW.")
    endif()

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

option(ENABLE_TESTS "Enable unit tests with Google Test" ON)

if(ENABLE_TESTS)
    include(FetchContent)
    
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(six_program 
    main.cpp
    Six.cpp
)

target_include_directories(six_program PRIVATE include)

if(ENABLE_TESTS)
    add_executable(six_tests
        test_six.cpp
        Six.cpp
    )

    target_include_directories(six_tests PRIVATE include)
    target_link_libraries(six_tests PRIVATE GTest::gtest_main)

    set_target_properties(six_tests PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        OUTPUT_NAME tests
    )

    add_custom_target(run_tests
        COMMAND $<TARGET_FILE:six_tests>
        DEPENDS six_tests
        COMMENT "Running unit tests..."
    )
endif()

set_target_properties(six_program PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME six
)

message(STATUS "Project ${PROJECT_NAME} configured successfully")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Tests enabled: ${ENABLE_TESTS}")