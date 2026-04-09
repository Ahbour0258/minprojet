// ============================================
// Cours : Constructeur & Destructeur
// Cours : Chaînes de caracteres en C++
// ============================================
#include "menu.h"

Menu::Menu(sf::Font& police)
    : _selection(0),
      _choix(ChoixMenu::Rien),
      _afficherAPropos(false)
{
    // Titre du jeu
    _titre.setFont(police);
    _titre.setString("HIDE & RUN");
    _titre.setCharacterSize(72);
    _titre.setFillColor(sf::Color(70, 170, 255));
    _titre.setStyle(sf::Text::Bold);
    _titre.setPosition(420.f, 100.f);

    // Items du menu - tableau de chaines
    // Cours : Chaines de caracteres & Tableaux
    const std::string libelles[3] = {
        "Jouer",
        "A propos",
        "Quitter"
    };

    for (int i = 0; i < 3; i++) {
        _items[i].setFont(police);
        _items[i].setString(libelles[i]);
        _items[i].setCharacterSize(40);
        _items[i].setPosition(560.f, 270.f + i * 80.f);
    }

    // Texte A propos
    _txtAPropos.setFont(police);
    _txtAPropos.setCharacterSize(26);
    _txtAPropos.setFillColor(sf::Color::White);
    _txtAPropos.setPosition(180.f, 160.f);
    _txtAPropos.setString(
        "=== HIDE & RUN ===\n\n"
        "Courez vers le bunker avant la fin du temps !\n\n"
        "Commandes :\n"
        "  Fleche Haut / Espace  : Sauter\n"
        "  Fleche Bas  / S       : S'accroupir\n\n"
        "Obstacles :\n"
        "  Cactus (sol)  : sauter par-dessus\n"
        "  Oiseaux (air) : s'accroupir pour esquiver\n\n"
        "Regles :\n"
        "  1er choc = blesse\n"
        "  2e choc  = mort\n\n"
        "  Echap : Retour au menu"
    );

    _mettreAJourCouleurs();
}

void Menu::gererEntree(const sf::Event& event) {
    if (_afficherAPropos) {
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape)
            _afficherAPropos = false;
        return;
    }

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up)
            _selection = (_selection + 2) % 3;
        if (event.key.code == sf::Keyboard::Down)
            _selection = (_selection + 1) % 3;
        if (event.key.code == sf::Keyboard::Return ||
            event.key.code == sf::Keyboard::Space) {
            if (_selection == 0) _choix = ChoixMenu::Jouer;
            if (_selection == 1) _afficherAPropos = true;
            if (_selection == 2) _choix = ChoixMenu::Quitter;
        }
    }
    _mettreAJourCouleurs();
}

void Menu::afficher(sf::RenderWindow& fen) const {
    fen.draw(_titre);
    if (_afficherAPropos) {
        fen.draw(_txtAPropos);
        return;
    }
    for (int i = 0; i < 3; i++)
        fen.draw(_items[i]);
}

void Menu::_mettreAJourCouleurs() {
    for (int i = 0; i < 3; i++)
        _items[i].setFillColor(
            i == _selection
            ? sf::Color(70, 170, 255)
            : sf::Color(210, 210, 210)
        );
}