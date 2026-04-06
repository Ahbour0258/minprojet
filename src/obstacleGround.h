#pragma once
#include "obstacle.h"
#include "constants.h"

class ObstacleGround : public Obstacle {
public:
    explicit ObstacleGround(float x);

    void          update(float dt)                    override;
    void          draw(sf::RenderWindow& win)  const  override;
    sf::FloatRect getBounds()                  const  override;
    std::string   getType()                    const  override { return "ground"; }

private:
    sf::RectangleShape _shape;
};