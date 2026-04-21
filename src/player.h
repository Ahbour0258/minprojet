#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"

enum class EtatJoueur { Vivant, Blesse, Mort };

//Style de jouueur
class Joueur {
  public:
    Joueur();
    ~Joueur() = default;

     void gererEntree(const sf::Event& ev);
     void mettreAJour(float dt);
     void afficher(sf::RenderWindow& fen) const;

     sf::FloatRect getBornes() const;
     EtatJoueur getEtat() const{
         return etat; }
     bool estMort() const{
         return etat == EtatJoueur::Mort; }
     void recevoirChoc();

 private:
    
    sf::RectangleShape tete; //bloc tete
    sf::RectangleShape corps; //bloc corps
    sf::RectangleShape brasDroit; //bloc bras droit
    sf::RectangleShape brasGauche; //bloc bras gauche
    sf::RectangleShape jambeD; //bloc jambe droite
    sf::RectangleShape jambeG; //bloc jambe gauche

    //Details du visage
    sf::RectangleShape oeilG;
    sf::RectangleShape oeilD;
    sf::RectangleShape bouche;    
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