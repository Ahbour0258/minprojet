// ============================================
// Cours : Constructeur & Destructeur
// Cours : Classe & Objet en C++
// ============================================
#include "obstacleAir.h"
#include <cstdlib>
#include <cmath>

ObstacleAir::ObstacleAir(float x)
    : _tempsVol(0.f)
{
    // Position aleatoire en hauteur
    _baseY = SOL_Y - 180.f - (std::rand() % 100);

    // Corps de l'oiseau
    _corps.setRadius(14.f);
    _corps.setOrigin(14.f, 14.f);
    _corps.setPosition(x, _baseY);
    _corps.setFillColor(sf::Color(80, 50, 20));
    _corps.setOutlineColor(sf::Color(50, 30, 10));
    _corps.setOutlineThickness(1.5f);

    // Aile gauche (triangle)
    _aile1.setPointCount(3);
    _aile1.setPoint(0, {0.f,   0.f});
    _aile1.setPoint(1, {-30.f, -15.f});
    _aile1.setPoint(2, {-25.f, 5.f});
    _aile1.setFillColor(sf::Color(60, 35, 10));
    _aile1.setOutlineColor(sf::Color(40, 20, 5));
    _aile1.setOutlineThickness(1.f);

    // Aile droite (triangle)
    _aile2.setPointCount(3);
    _aile2.setPoint(0, {0.f,  0.f});
    _aile2.setPoint(1, {30.f, -15.f});
    _aile2.setPoint(2, {25.f, 5.f});
    _aile2.setFillColor(sf::Color(60, 35, 10));
    _aile2.setOutlineColor(sf::Color(40, 20, 5));
    _aile2.setOutlineThickness(1.f);
}

void ObstacleAir::deplacer(float dt) {
    _tempsVol += dt;

    // Mouvement sinusoidal (montee/descente)
    float nouvelleY = _baseY + std::sin(_tempsVol * 2.5f) * 18.f;
    float batte     = std::sin(_tempsVol * 8.f) * 10.f;

    _corps.setPosition(
        _corps.getPosition().x - VITESSE_OBS * dt,
        nouvelleY
    );

    // Animation des ailes
    sf::Vector2f pos = _corps.getPosition();
    _aile1.setPosition(pos.x - 2.f, pos.y + batte);
    _aile2.setPosition(pos.x + 2.f, pos.y + batte);
}

void ObstacleAir::afficher(sf::RenderWindow& fen) const {
    fen.draw(_aile1);
    fen.draw(_aile2);
    fen.draw(_corps);
    // Oeil de l'oiseau
    sf::CircleShape oeil(3.f);
    oeil.setOrigin(3.f, 3.f);
    oeil.setPosition(
        _corps.getPosition().x + 8.f,
        _corps.getPosition().y - 4.f
    );
    oeil.setFillColor(sf::Color::White);
    sf::CircleShape pupille(1.5f);
    pupille.setOrigin(1.5f, 1.5f);
    pupille.setPosition(
        _corps.getPosition().x + 9.f,
        _corps.getPosition().y - 4.f
    );
    pupille.setFillColor(sf::Color::Black);
    fen.draw(oeil);
    fen.draw(pupille);
}

sf::FloatRect ObstacleAir::getBornes() const {
    return _corps.getGlobalBounds();
}