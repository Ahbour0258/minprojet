#include "gameManager.h"
#include <iostream>
#include <stdexcept>

int main() {
    try {
        GameManager game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "[FATAL] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}