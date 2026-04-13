#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"

enum class EtatJoueur { Vivant, Blesse, Mort };

// Joueur style Minecraft : corps en blocs carres
class Joueur {
  public:
    Joueur();
    ~Joueur() = default;

     void gererEntree(const sf::Event& ev);
     void mettreAJour(float dt);
     void afficher(sf::RenderWindow& fen) const;

     sf::FloatRect getBornes() const;
     EtatJoueur getEtat() const {
         return etat; }
     bool estMort() const {
         return etat == EtatJoueur::Mort; }
     void recevoirChoc();

 private:
    // Corps style Minecraft (blocs carres)
    sf::RectangleShape tete;       // bloc tete
    sf::RectangleShape corps;      // bloc corps
    sf::RectangleShape brasDroit;  // bloc bras droit
    sf::RectangleShape brasGauche; // bloc bras gauche
    sf::RectangleShape jambeD;     // bloc jambe droite
    sf::RectangleShape jambeG;     // bloc jambe gauche

    // Details du visage (pixel art)
    sf::RectangleShape oeilG;      // oeil gauche
    sf::RectangleShape oeilD;      // oeil droit
    sf::RectangleShape bouche;     // sourire
    // Cheveux (bloc sur la tete)
    sf::RectangleShape cheveux;
    sf::Vector2f vitesse;
    EtatJoueur etat;
    float timerInv;
    bool surSol;
    bool accroupi;
    int frameAnim;

    void sauter();
    void sAccroupir(bool actif);
    void majCouleur();
    void majPositions(float baseY);
};