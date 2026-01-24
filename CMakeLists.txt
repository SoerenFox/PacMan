cmake_minimum_required(VERSION 3.16)
project(PacMan VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(BUILD_EXAMPLES OFF)
set(BUILD_GAMES OFF)

FetchContent_MakeAvailable(raylib)

add_executable(PacMan
  main.cpp
  Game.cpp
  Entity.cpp
  Pacman.cpp
  Ghost.cpp
  Blinky.cpp
  Pinky.cpp
  Inky.cpp
  Clyde.cpp
  Map.cpp
  Leaderboard.cpp
  Display.cpp
  Audio.cpp
)

target_link_libraries(PacMan PRIVATE raylib)

add_custom_command(TARGET PacMan PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CMAKE_SOURCE_DIR}/sounds"
  "$<TARGET_FILE_DIR:PacMan>/sounds")

