#include "obstacleAir.h"
#include <cstdlib>
#include <cmath>

ObstacleAir::ObstacleAir(float x)
    : tempsVol(0.f)
{
    // Position aleatoire en hauteur
    baseY = SOL_Y - 180.f - (std::rand() % 100);

    // Corps de l'oiseau
    corps.setRadius(14.f);
    corps.setOrigin(14.f, 14.f);
    corps.setPosition(x, baseY);
    corps.setFillColor(sf::Color(80, 50, 20));
    corps.setOutlineColor(sf::Color(50, 30, 10));
    corps.setOutlineThickness(1.5f);

    // Aile gauche (triangle)
    aile1.setPointCount(3);
    aile1.setPoint(0, {0.f,   0.f});
    aile1.setPoint(1, {-30.f, -15.f});
    aile1.setPoint(2, {-25.f, 5.f});
    aile1.setFillColor(sf::Color(60, 35, 10));
    aile1.setOutlineColor(sf::Color(40, 20, 5));
    aile1.setOutlineThickness(1.f);

    // Aile droite (triangle)
    aile2.setPointCount(3);
    aile2.setPoint(0, {0.f,  0.f});
    aile2.setPoint(1, {30.f, -15.f});
    aile2.setPoint(2, {25.f, 5.f});
    aile2.setFillColor(sf::Color(60, 35, 10));
    aile2.setOutlineColor(sf::Color(40, 20, 5));
    aile2.setOutlineThickness(1.f);
}

void ObstacleAir::deplacer(float dt) {
    tempsVol += dt;

    // Mouvement sinusoidal (montee/descente)
    float nouvelleY = baseY + std::sin(tempsVol * 2.5f) * 18.f;
    float batte     = std::sin(tempsVol * 8.f) * 10.f;

    corps.setPosition(
        corps.getPosition().x - VITESSE_OBS * dt,
        nouvelleY
    );

    // Animation des ailes
    sf::Vector2f pos =corps.getPosition();
    aile1.setPosition(pos.x - 2.f, pos.y + batte);
    aile2.setPosition(pos.x + 2.f, pos.y + batte);
}

void ObstacleAir::afficher(sf::RenderWindow& fen) const {
    fen.draw(aile1);
    fen.draw(aile2);
    fen.draw(corps);
    // Oeil de l'oiseau
    sf::CircleShape oeil(3.f);
    oeil.setOrigin(3.f, 3.f);
    oeil.setPosition(
        corps.getPosition().x + 8.f,
        corps.getPosition().y - 4.f
    );
    oeil.setFillColor(sf::Color::White);
    sf::CircleShape pupille(1.5f);
    pupille.setOrigin(1.5f, 1.5f);
    pupille.setPosition(
     corps.getPosition().x + 9.f,
     corps.getPosition().y - 4.f
    );
    pupille.setFillColor(sf::Color::Black);
    fen.draw(oeil);
    fen.draw(pupille);
}

sf::FloatRect ObstacleAir::getBornes() const {
    return corps.getGlobalBounds();
}