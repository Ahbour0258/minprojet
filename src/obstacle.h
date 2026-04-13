#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Obstacle {
public:
    virtual ~Obstacle()=default;
    virtual void deplacer(float dt)=0;
    virtual void afficher(sf::RenderWindow& fen) const=0;
    virtual sf::FloatRect getBornes() const=0;
    virtual std::string getType() const=0;

    bool horsEcran() const {
        return getBornes().left +getBornes().width < 0.f;
    }
};