#include "obstacleGround.h"
#include <cstdlib>

ObstacleGround::ObstacleGround(float x) {
    float h = 30.f + std::rand() % 20;
    _shape.setSize({38.f, h});
    _shape.setOrigin(19.f, h);
    _shape.setPosition(x, GROUND_Y);
    _shape.setFillColor(sf::Color(180, 60, 60));
    _shape.setOutlineColor(sf::Color(255, 80, 80));
    _shape.setOutlineThickness(2.f);
}

void ObstacleGround::update(float dt) {
    _shape.move(-OBS_SPEED * dt, 0.f);
}

void ObstacleGround::draw(sf::RenderWindow& win) const {
    win.draw(_shape);
}

sf::FloatRect ObstacleGround::getBounds() const {
    return _shape.getGlobalBounds();
}