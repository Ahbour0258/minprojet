// ============================================
// Cours : Classe & Objet + Heritage en C++
// Activite : Obstacle au sol - Cactus
// ============================================
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
    void          deplacer(float dt)                override;
    void          afficher(sf::RenderWindow& fen)
                                             const  override;
    sf::FloatRect getBornes()                const  override;
    std::string   getType()                  const  override {
        return "sol";
    }

private:
    // Encapsulation - attributs prives
    sf::RectangleShape _corps;    // corps du cactus
    sf::RectangleShape _bras1;    // bras gauche
    sf::RectangleShape _bras2;    // bras droit
};