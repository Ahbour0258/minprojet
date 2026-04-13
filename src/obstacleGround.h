#pragma once
#include "obstacle.h"
#include "constants.h"

class ObstacleSol : public Obstacle {

 public:
    explicit ObstacleSol(float x);
    void deplacer(float dt) override;
    void afficher(sf::RenderWindow& fen) const override;
    sf::FloatRect getBornes() const override;
    std::string getType() const override { 
        return "sol"; }

 private:
    sf::RectangleShape corps;
    sf::RectangleShape bras1;
    sf::RectangleShape bras2;
};