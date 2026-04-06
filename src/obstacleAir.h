#pragma once
#include "obstacle.h"
#include "constants.h"

class ObstacleAir : public Obstacle {
public:
    explicit ObstacleAir(float x);

    void          update(float dt)                   override;
    void          draw(sf::RenderWindow& win) const  override;
    sf::FloatRect getBounds()                const   override;
    std::string   getType()                  const   override { return "air"; }

private:
    sf::CircleShape _shape;
    float           _floatTimer;
    float           _baseY;
};