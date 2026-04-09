cmake_minimum_required(VERSION 3.16)
project(HideAndRun)

set(CMAKE_CXX_STANDARD 17)

# Trouver SFML
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Lister TOUS les fichiers sources
add_executable(HideAndRun
    src/main.cpp
    src/gameManager.cpp
    src/player.cpp         # IMPORTANT : doit être présent
    src/menu.cpp
    src/obstacle.cpp
    src/obstacleAir.cpp
    src/obstacleGround.cpp
)

# Lier les bibliothèques SFML
target_link_libraries(HideAndRun sfml-graphics sfml-window sfml-system sfml-audio)