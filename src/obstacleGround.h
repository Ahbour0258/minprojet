#pragma once
#include "obstacle.h"
#include "constants.h"

// Obstacle au sol : Cactus
// Heritage public de Obstacle
class ObstacleSol : public Obstacle {
public:
    // Constructeur - Cours : Constructeur & Destructeur
    explicit ObstacleSol(float x);

    // Surcharge des methodes virtuelles
    void deplacer(float dt) override;
    void afficher(sf::RenderWindow& fen)
    const override;
    sf::FloatRect getBornes() const override;
    std::string getType() const override {
        return "sol";
    }

private:
    // Encapsulation - attributs prives
    sf::RectangleShape corps;    // corps du cactus
    sf::RectangleShape bras1;    // bras gauche
    sf::RectangleShape bras2;    // bras droit
};