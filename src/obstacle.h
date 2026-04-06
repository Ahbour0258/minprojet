#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Obstacle {
public:
    virtual ~Obstacle() = default;

    virtual void          update(float dt)                  = 0;
    virtual void          draw(sf::RenderWindow& win) const = 0;
    virtual sf::FloatRect getBounds()                 const = 0;
    virtual std::string   getType()                   const = 0;

    bool isOffScreen() const {
        return getBounds().left + getBounds().width < 0.f;
    }
};