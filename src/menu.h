#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Choix du menu - enum
enum class ChoixMenu { Rien, Jouer, APropos, Quitter };

class Menu {
public:
    explicit Menu(sf::Font& police);
    void gererEntree(const sf::Event& event);
    void afficher(sf::RenderWindow& fen) const;
    ChoixMenu getChoix() const { return choix; }
    void resetChoix() { choix = ChoixMenu::Rien; }

private:
    sf::Text titre;
    sf::Text items[3];   // Tableau C++ - cours STL
    sf::Text txtAPropos;
    int selection;
    ChoixMenu choix;
    bool afficherAPropos;

    void mettreAJourCouleurs();
};