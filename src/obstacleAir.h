#pragma once
#include "obstacle.h"
#include "constants.h"

// Obstacle en l'air : Oiseau (code original)
class ObstacleAir : public Obstacle {
public:
    explicit ObstacleAir(float x);
    void deplacer(float dt) override;
    void afficher(sf::RenderWindow& fen) const override;
    sf::FloatRect getBornes() const override;
    std::string getType() const override { return "air"; }

private:
    sf::CircleShape corps;
    sf::ConvexShape aile1;
    sf::ConvexShape aile2;
    sf::CircleShape oeil;
    sf::CircleShape pupille;
    float tempsVol;
    float baseY;
};