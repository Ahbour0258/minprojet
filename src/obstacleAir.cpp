#include "obstacleAir.h"
#include "constants.h"
#include <cmath>

ObstacleAir::ObstacleAir(float x)
    : tempsVol(0.f), baseY(POS_SOL - 100.f)
{
    // Corps de l'oiseau
    corps.setRadius(14.f);
    corps.setOrigin(14.f, 14.f);
    corps.setPosition(x, baseY);
    corps.setFillColor(sf::Color(180, 80, 80));
    // Oeil
    oeil.setRadius(4.f);
    oeil.setOrigin(4.f, 4.f);
    oeil.setPosition(x + 6.f, baseY - 6.f);
    oeil.setFillColor(sf::Color::White);
    // Pupille
  pupille.setRadius(2.f);
  pupille.setOrigin(2.f, 2.f);
  pupille.setPosition(x + 7.f, baseY - 6.f);
  pupille.setFillColor(sf::Color::Black);

    // Aile gauche
    aile1.setPointCount(3);
    aile1.setPoint(0, sf::Vector2f(0.f,    0.f));
    aile1.setPoint(1, sf::Vector2f(-22.f, -12.f));
    aile1.setPoint(2, sf::Vector2f(-22.f,  4.f));
    aile1.setFillColor(sf::Color(140, 60, 60));
    aile1.setPosition(x, baseY);

    // Aile droite
    aile2.setPointCount(3);
    aile2.setPoint(0, sf::Vector2f(0.f,   0.f));
    aile2.setPoint(1, sf::Vector2f(22.f, -12.f));
    aile2.setPoint(2, sf::Vector2f(22.f,  4.f));
    aile2.setFillColor(sf::Color(140, 60, 60));
    aile2.setPosition(x, baseY);
}

void ObstacleAir::deplacer(float dt) {
    tempsVol += dt;

    float x = corps.getPosition().x - VITESSE_OBS * dt;
    float y = baseY + std::sin(tempsVol * 4.f) * 18.f;

    corps.setPosition(x, y);

    // Battement des ailes avec sin
    float battement = std::sin(tempsVol * 8.f) * 8.f;
    aile1.setPosition(x, y - battement);
    aile2.setPosition(x, y - battement);
    oeil.setPosition(x + 6.f, y - 6.f);
    pupille.setPosition(x + 7.f, y - 6.f);
}

void ObstacleAir::afficher(sf::RenderWindow& fen) const {
    fen.draw(aile1);
    fen.draw(aile2);
    fen.draw(corps);
    fen.draw(oeil);
    fen.draw(pupille);
}

sf::FloatRect ObstacleAir::getBornes() const {
    return corps.getGlobalBounds();
}