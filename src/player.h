// ============================================
// Cours : Classe & Objet en C++
// Cours : Constructeur & Destructeur
// Activite : Gestion du joueur
// ============================================
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
    sf::FloatRect getBornes()   const;
    EtatJoueur    getEtat()     const { return _etat; }
    bool          estMort()     const {
        return _etat == EtatJoueur::Mort;
    }

    // Gestion des chocs
    void recevoirChoc();

private:
    // Attributs prives - encapsulation
    sf::RectangleShape _corps;      // corps du personnage
    sf::CircleShape    _tete;       // tete
    sf::RectangleShape _jambe1;     // jambe gauche
    sf::RectangleShape _jambe2;     // jambe droite
    sf::Vector2f       _vitesse;
    EtatJoueur         _etat;
    float              _timerInv;   // timer invincibilite
    bool               _surSol;
    bool               _accroupi;
    int                _frameAnim;  // frame animation course

    // Methodes privees
    void _sauter();
    void _sAccroupir(bool actif);
    void _mettreAJourCouleur();
};