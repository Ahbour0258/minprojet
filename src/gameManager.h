// ============================================
// Cours : Les Tableaux & la Bibliotheque STL
// Cours : Fonctions amies & Patrons en C++
// Activite : Gestionnaire principal du jeu
// ============================================
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
    sf::RenderWindow  _fenetre;
    sf::Font          _police;
    sf::Texture       _textureFond;
    sf::Sprite        _fond;
    sf::Sprite        _fond2;     // double fond pour scrolling

    // -- Textes HUD --
    sf::Text          _txtChrono;
    sf::Text          _txtEtat;
    sf::Text          _txtMessage;

    // -- Formes --
    sf::RectangleShape _sol;
    sf::RectangleShape _barreProgres;
    sf::RectangleShape _remplissage;

    // -- Objets du jeu --
    // Cours : Agregation
    Joueur            _joueur;
    // STL vector + pointeurs - cours STL & Pointeurs
    std::vector<std::unique_ptr<Obstacle>> _obstacles;
    Menu              _menu;
    EtatJeu           _etatJeu;

    // -- Variables --
    float _chrono;
    float _timerSpawn;
    float _prochainSpawn;
    float _offsetFond;

    // -- Methodes privees --
    void _gererEvenements();
    void _mettreAJour(float dt);
    void _afficher();
    void _genererObstacle();
    void _verifierCollisions();
    void _afficherHUD();
    void _dessinerFond();
    void _reinitialiser();

    // Patron de fonction - cours Patrons
    // Cours : Fonctions, Pointeur et Reference
    float _aleatoire(float min, float max);
};