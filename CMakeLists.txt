cmake_minimum_required(VERSION 4.0)

project(game_engine_2d VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Lua REQUIRED)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

set(EXE game_engine)
add_executable(${EXE})

target_include_directories(${EXE} SYSTEM PRIVATE libs)
target_link_libraries(${EXE} PRIVATE SDL2 SDL2_image SDL2_ttf)
target_link_libraries(${EXE} PRIVATE lua5.3)

add_subdirectory(src)

add_library(imgui STATIC
    libs/imgui/imgui.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui_demo.cpp
    libs/imgui/imgui_impl_sdl2.cpp
    libs/imgui/imgui_impl_sdlrenderer2.cpp 
)

target_include_directories(imgui SYSTEM PRIVATE libs/imgui libs/imgui/backends)

target_compile_options(imgui PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-w>
    $<$<CXX_COMPILER_ID:Clang>:-w>
    $<$<CXX_COMPILER_ID:MSVC>:/w>
)

target_include_directories(imgui PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(imgui PRIVATE SDL2)

target_link_libraries(${EXE} PRIVATE imgui)


