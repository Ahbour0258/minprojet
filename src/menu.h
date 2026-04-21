#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class ChoixMenu{ Rien, Jouer, Quitter };

class Menu {
 public:
    Menu() = default;
    explicit Menu(sf::Font& police);

    void gererEntree(const sf::Event& event);
    void afficher(sf::RenderWindow& fen) const;
    ChoixMenu getChoix() const { return choix; }
    void resetChoix(){ choix = ChoixMenu::Rien; } //Met à jour l’état du menu 
 
 private:
      sf::Text titre; //Titre affiché dans le menu
      sf::Text items[3];
      sf::Text txtAPropos;
      sf::RectangleShape fondsItems[3];
      sf::RectangleShape fondTitre;
      sf::RectangleShape ligne;

      int selection;
      ChoixMenu choix;
       bool afficherAPropos;

    void mettreAJourCouleurs();
};