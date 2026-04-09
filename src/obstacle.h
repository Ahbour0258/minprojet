// ============================================
// Cours : Classe & Objet en C++
// Cours : Transmission d'objet en argument
// Activite : Classe abstraite - Polymorphisme
// ============================================
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Classe abstraite - base de tous les obstacles
class Obstacle {
public:
    // Destructeur virtuel (bonne pratique POO)
    virtual ~Obstacle() = default;

    // Methodes virtuelles pures = interface commune
    virtual void          deplacer(float dt)               = 0;
    virtual void          afficher(sf::RenderWindow& fen)
                                                     const = 0;
    virtual sf::FloatRect getBornes()              const   = 0;
    virtual std::string   getType()                const   = 0;

    // Methode commune a tous les obstacles
    bool horsEcran() const {
        return getBornes().left + getBornes().width < 0.f;
    }
};