#include "obstacleGround.h"
#include <cstdlib>

// Constructeur du cactus
ObstacleSol::ObstacleSol(float x) {
    // Corps principal du cactus
    corps.setSize({22.f, 55.f});
    corps.setOrigin(11.f, 55.f);
    corps.setPosition(x, SOL_Y);
    corps.setFillColor(sf::Color(34, 139, 34));
    corps.setOutlineColor(sf::Color(0, 100, 0));
    corps.setOutlineThickness(2.f);

    // Bras gauche du cactus
    bras1.setSize({18.f, 12.f});
    bras1.setOrigin(18.f, 6.f);
    bras1.setPosition(x - 3.f, SOL_Y - 30.f);
    bras1.setFillColor(sf::Color(34, 139, 34));
    bras1.setOutlineColor(sf::Color(0, 100, 0));
    bras1.setOutlineThickness(2.f);

    // Bras droit du cactus
    bras2.setSize({18.f, 12.f});
    bras2.setOrigin(0.f, 6.f);
    bras2.setPosition(x + 3.f, SOL_Y - 22.f);
    bras2.setFillColor(sf::Color(34, 139, 34));
    bras2.setOutlineColor(sf::Color(0, 100, 0));
    bras2.setOutlineThickness(2.f);
}

// Deplacement vers la gauche
void ObstacleSol::deplacer(float dt) {
    corps.move(-VITESSE_OBS * dt, 0.f);
    bras1.move(-VITESSE_OBS * dt, 0.f);
    bras2.move(-VITESSE_OBS * dt, 0.f);
}

// Affichage du cactus
void ObstacleSol::afficher(sf::RenderWindow& fen) const {
    fen.draw(bras1);
    fen.draw(bras2);
    fen.draw(corps);
}

// Boite de collision
sf::FloatRect ObstacleSol::getBornes() const {
    return corps.getGlobalBounds();
}