cmake_minimum_required(VERSION 3.15)
project(NPC_Game VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Для MinGW
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Список всех файлов
set(SOURCES
    main.cpp
    implementations.cpp
    game.cpp
)

set(HEADERS
    game.h
    npc.h
    npc_types.h
    threaded_game.h
    dungeon_editor.h
    game_constants.h
    position.h
    observer.h
    visitor.h
    factory.h
)

# Убедимся, что файлы существуют
foreach(header ${HEADERS})
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${header})
        message(FATAL_ERROR "Required header file ${header} not found!")
    endif()
endforeach()

add_executable(${PROJECT_NAME} ${SOURCES})

# Для Windows
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()