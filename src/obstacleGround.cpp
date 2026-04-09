// ============================================
// Cours : Constructeur & Destructeur
// Cours : Classe & Objet en C++
// ============================================
#include "obstacleGround.h"
#include <cstdlib>

// Constructeur du cactus
ObstacleSol::ObstacleSol(float x) {
    // Corps principal du cactus
    _corps.setSize({22.f, 55.f});
    _corps.setOrigin(11.f, 55.f);
    _corps.setPosition(x, SOL_Y);
    _corps.setFillColor(sf::Color(34, 139, 34));
    _corps.setOutlineColor(sf::Color(0, 100, 0));
    _corps.setOutlineThickness(2.f);

    // Bras gauche du cactus
    _bras1.setSize({18.f, 12.f});
    _bras1.setOrigin(18.f, 6.f);
    _bras1.setPosition(x - 3.f, SOL_Y - 30.f);
    _bras1.setFillColor(sf::Color(34, 139, 34));
    _bras1.setOutlineColor(sf::Color(0, 100, 0));
    _bras1.setOutlineThickness(2.f);

    // Bras droit du cactus
    _bras2.setSize({18.f, 12.f});
    _bras2.setOrigin(0.f, 6.f);
    _bras2.setPosition(x + 3.f, SOL_Y - 22.f);
    _bras2.setFillColor(sf::Color(34, 139, 34));
    _bras2.setOutlineColor(sf::Color(0, 100, 0));
    _bras2.setOutlineThickness(2.f);
}

// Deplacement vers la gauche
void ObstacleSol::deplacer(float dt) {
    _corps.move(-VITESSE_OBS * dt, 0.f);
    _bras1.move(-VITESSE_OBS * dt, 0.f);
    _bras2.move(-VITESSE_OBS * dt, 0.f);
}

// Affichage du cactus
void ObstacleSol::afficher(sf::RenderWindow& fen) const {
    fen.draw(_bras1);
    fen.draw(_bras2);
    fen.draw(_corps);
}

// Boite de collision
sf::FloatRect ObstacleSol::getBornes() const {
    return _corps.getGlobalBounds();
}