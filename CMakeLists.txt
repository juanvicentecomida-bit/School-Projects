cmake_minimum_required(VERSION 3.10)
project(Tetris VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)  # Changed from 14 to 17 for filesystem
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/core/game.cpp
    src/core/game_state.cpp
    src/entities/tetromino.cpp
    src/entities/playfield.cpp
    src/input/input_handler.cpp
    src/scoring/score_manager.cpp
    src/scoring/high_score.cpp
    src/rendering/renderer.cpp
    src/config/config_manager.cpp  # NEW
)

# Executable
add_executable(tetris ${SOURCES})

# Include directories
target_include_directories(tetris PRIVATE
    ${CMAKE_SOURCE_DIR}/include  # NEW - for json.hpp
    src
    src/core
    src/entities
    src/input
    src/scoring
    src/rendering
    src/utils
    src/config  # NEW
)

# Linux-specific libraries
if(UNIX)
    target_link_libraries(tetris pthread)  # Added stdc++fs for filesystem
endif()

# Copy config directory to build folder
add_custom_command(TARGET tetris POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:tetris>/config
    COMMENT "Copying config files to build directory"
)

# Install targets
install(TARGETS tetris DESTINATION bin)
install(DIRECTORY config DESTINATION bin)

# Custom run target
add_custom_target(run
    COMMAND tetris
    DEPENDS tetris
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")