// ============================================
// Cours : Les bases de C++ - Point d'entree
// Cours : Gestion des exceptions try/catch
// Mini Projet C++ - Hide & Run
// ============================================
#include "gameManager.h"
#include <iostream>
#include <stdexcept>

int main() {
    // Gestion des exceptions - cours bases C++
    try {
        GestionnaireJeu jeu;
        jeu.lancer();
    } catch (const std::exception& e) {
        std::cerr << "[FATAL] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}