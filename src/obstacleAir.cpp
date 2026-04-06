#include "obstacleAir.h"
#include <cstdlib>
#include <cmath>

ObstacleAir::ObstacleAir(float x)
    : _floatTimer(0.f)
{
    float yPos = GROUND_Y - 160.f - std::rand() % 80;
    _baseY = yPos;
    _shape.setRadius(22.f);
    _shape.setOrigin(22.f, 22.f);
    _shape.setPosition(x, yPos);
    _shape.setFillColor(sf::Color(60, 60, 200));
    _shape.setOutlineColor(sf::Color(100, 150, 255));
    _shape.setOutlineThickness(2.f);
}

void ObstacleAir::update(float dt) {
    _floatTimer += dt;
    float newY = _baseY + std::sin(_floatTimer * 2.f) * 15.f;
    _shape.setPosition(
        _shape.getPosition().x - OBS_SPEED * dt,
        newY
    );
}

void ObstacleAir::draw(sf::RenderWindow& win) const {
    win.draw(_shape);
}

sf::FloatRect ObstacleAir::getBounds() const {
    return _shape.getGlobalBounds();
}