#pragma once
#include "obstacle.h"
#include "constants.h"

// Obstacle en l'air : Oiseau
class ObstacleAir : public Obstacle {
public:
    explicit ObstacleAir(float x);
    void deplacer(float dt) override;
    void afficher(sf::RenderWindow& fen)
    const override;
    sf::FloatRect getBornes() const override;
    std::string getType() const override {
        return "air";
    }

private:
    sf::CircleShape corps;     // corps de l'oiseau
    sf::ConvexShape aile1;     // aile gauche
    sf::ConvexShape aile2;     // aile droite
    float tempsVol;  // pour animation ailes
    float baseY;     // position Y de base
};