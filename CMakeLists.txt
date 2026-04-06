cmake_minimum_required(VERSION 3.16)
project(HideAndRun)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS
    graphics window audio system REQUIRED)

add_executable(HideAndRun
    src/main.cpp
    src/player.cpp
    src/obstacle.cpp
    src/obstacleGround.cpp
    src/obstacleAir.cpp
    src/gameManager.cpp
    src/menu.cpp
)

target_link_libraries(HideAndRun
    sfml-graphics sfml-window
    sfml-audio sfml-system
)

target_include_directories(HideAndRun PRIVATE src/)
