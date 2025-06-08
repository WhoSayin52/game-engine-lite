cmake_minimum_required(VERSION 4.0)

project(game_engine_2d VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

set(EXE game_engine)
add_executable(${EXE})

target_link_libraries(${EXE} PRIVATE SDL2 SDL2_image)

add_subdirectory(src)
