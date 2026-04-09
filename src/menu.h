// ============================================
// Cours : Classe & Objet en C++
// Cours : Les Tableaux & STL en C++
// Activite : Menu principal du jeu
// ============================================
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Choix du menu - enum
enum class ChoixMenu { Rien, Jouer, APropos, Quitter };

class Menu {
public:
    explicit Menu(sf::Font& police);

    void      gererEntree(const sf::Event& event);
    void      afficher(sf::RenderWindow& fen) const;
    ChoixMenu getChoix()    const { return _choix; }
    void      resetChoix()        { _choix = ChoixMenu::Rien; }

private:
    sf::Text  _titre;
    sf::Text  _items[3];   // Tableau C++ - cours STL
    sf::Text  _txtAPropos;
    int       _selection;
    ChoixMenu _choix;
    bool      _afficherAPropos;

    void _mettreAJourCouleurs();
};