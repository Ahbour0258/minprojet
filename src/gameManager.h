#pragma once
#include <SFML/Graphics.hpp>
#include <vector>        // STL - cours Tableaux & STL
#include <memory>        // unique_ptr
#include "player.h"    // agregation - cours Agregation
#include "obstacle.h"
#include "menu.h"
#include "constants.h"

// Etats possibles du jeu
enum class EtatJeu { Menu, EnJeu, GameOver, Victoire };

// Cours : Agregation et composition
class GestionnaireJeu {
public:
    GestionnaireJeu();   // Constructeur
    ~GestionnaireJeu();  // Destructeur
    void lancer();       // Boucle principale

private:
    // -- Agregation SFML --
    sf::RenderWindow fenetre;
    sf::Font police;
    sf::Texture textureFond;
    sf::Sprite fond;
    sf::Sprite fond2;     // double fond pour scrolling

    // -- Textes HUD --
    sf::Text txtChrono;
    sf::Text txtEtat;
    sf::Text txtMessage;

    // -- Formes --
    sf::RectangleShape sol;
    sf::RectangleShape barreProgres;
    sf::RectangleShape remplissage;

    // -- Objets du jeu --
    // Cours : Agregation
    Joueur joueur;
    // STL vector + pointeurs - cours STL & Pointeurs
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    Menu menu;
    EtatJeu etatJeu;

    // -- Variables --
    float chrono;
    float timerSpawn;
    float prochainSpawn;
    float offsetFond;

    // -- Methodes privees --
    void gererEvenements();
    void mettreAJour(float dt);
    void afficher();
    void genererObstacle();
    void verifierCollisions();
    void afficherHUD();
    void dessinerFond();
    void reinitialiser();

    // Patron de fonction - cours Patrons
    // Cours : Fonctions, Pointeur et Reference
    float aleatoire(float min, float max);
};