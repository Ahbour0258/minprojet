cmake_minimum_required(VERSION 3.16)
project(HideAndRun)

set(CMAKE_CXX_STANDARD 17)

# Chemin SFML + mode dynamique (obligatoire avec openal32.dll)
set(SFML_DIR "C:/SFML-GCC/lib/cmake/SFML")
set(SFML_STATIC_LIBRARIES FALSE)

# Trouver SFML
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Lister TOUS les fichiers sources
add_executable(JeuCachette
    src/main.cpp
    src/gameManager.cpp
    src/player.cpp
    src/menu.cpp
    src/obstacle.cpp
    src/obstacleAir.cpp
    src/obstacleGround.cpp
)

# Lier les bibliothèques SFML
target_link_libraries(JeuCachette
    sfml-graphics 
    sfml-window 
    sfml-system 
    sfml-audio
    opengl32
    winmm
    gdi32
)