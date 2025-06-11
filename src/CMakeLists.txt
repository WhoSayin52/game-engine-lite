target_sources(${EXE} PRIVATE main.cpp)

add_subdirectory(components)
add_subdirectory(ecs)
add_subdirectory(game)
add_subdirectory(logger)
add_subdirectory(systems)
