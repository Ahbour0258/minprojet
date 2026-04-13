#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "player.h"
#include "obstacle.h"
#include "menu.h"
#include "constants.h"

enum class EtatJeu { Menu, EnJeu, GameOver, Victoire };

class GestionnaireJeu {
public:
    GestionnaireJeu();
    ~GestionnaireJeu();
    void lancer();

private:
    sf::RenderWindow fenetre; //référence à la fenêtre principale du jeu. 
    sf::Font police;
    sf::Texture texFond;
    sf::Sprite fond;
    sf::Text txtChrono;
    sf::Text txtEtat;
    sf::Text txtMessage;
    sf::RectangleShape sol;

    Joueur joueur;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    Menu menu;
    EtatJeu etatJeu;

    float chrono;
    float timerSpawn;
    float prochainSpawn;
    float offsetFond;

    void gererEvenements();
    void mettreAJour(float dt);
    void afficher();
    void genererObstacle();
    void verifierCollisions();
    void afficherHUD();
    void dessinerFond();
    void reinitialiser();
    float aleatoire(float min, float max);
};