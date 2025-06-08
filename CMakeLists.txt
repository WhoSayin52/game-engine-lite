cmake_minimum_required(VERSION 4.0)

project(game_engine_2d VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

set(EXE game_engine)
add_executable(${EXE})

target_link_libraries(${EXE} PRIVATE SDL2::SDL2)

add_subdirectory(src)
