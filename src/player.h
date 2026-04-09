#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"

// Etats possibles du joueur
// Cours : Les bases de C++ - enum class
enum class EtatJoueur { Vivant, Blesse, Mort };

class Joueur {
public:
    // Constructeur & Destructeur
    Joueur();
    ~Joueur() = default;

    // Methodes publiques
    void gererEntree(const sf::Event& event);
    void mettreAJour(float dt);
    void afficher(sf::RenderWindow& fen) const;

    // Getters - encapsulation
    sf::FloatRect getBornes() const;
    EtatJoueur getEtat() const { return etat; }
    bool estMort() const {
        return etat == EtatJoueur::Mort;
    }

    // Gestion des chocs
    void recevoirChoc();

private:
    // Attributs prives - encapsulation
    sf::RectangleShape corps;      // corps du personnage
    sf::CircleShape tete;       // tete
    sf::RectangleShape jambe1;     // jambe gauche
    sf::RectangleShape jambe2;     // jambe droite
    sf::Vector2f vitesse;
    EtatJoueur etat;
    float timerInv;   // timer invincibilite
    bool surSol;
    bool accroupi;
    int frameAnim;  // frame animation course

    // Methodes privees
    void sauter();
    void sAccroupir(bool actif);
    void mettreAJourCouleur();
};