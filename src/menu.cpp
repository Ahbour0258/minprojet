#include "menu.h"

Menu::Menu(sf::Font& font)
    : _selected(0),
      _choice(MenuChoice::None),
      _showAbout(false)
{
    _title.setFont(font);
    _title.setString("HIDE & RUN");
    _title.setCharacterSize(64);
    _title.setFillColor(sf::Color(70, 170, 255));
    _title.setPosition(440.f, 120.f);

    const char* labels[] = {"Jouer", "A propos", "Quitter"};
    for (int i = 0; i < 3; i++) {
        _items[i].setFont(font);
        _items[i].setString(labels[i]);
        _items[i].setCharacterSize(36);
        _items[i].setPosition(560.f, 280.f + i * 70.f);
    }

    _about.setFont(font);
    _about.setString(
        "HIDE & RUN\n\n"
        "Courez vers le bunker avant la fin du temps !\n"
        "Sautez (Haut/Espace) pour eviter les obstacles au sol.\n"
        "Baissez-vous (Bas/S) pour esquiver les drones.\n\n"
        "Deux chocs = game over.\n\n"
        "Appuyez sur Echap pour revenir."
    );
    _about.setCharacterSize(24);
    _about.setFillColor(sf::Color::White);
    _about.setPosition(200.f, 200.f);

    _updateColors();
}

void Menu::handleInput(const sf::Event& event) {
    if (_showAbout) {
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape)
            _showAbout = false;
        return;
    }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up)
            _selected = (_selected + 2) % 3;
        if (event.key.code == sf::Keyboard::Down)
            _selected = (_selected + 1) % 3;
        if (event.key.code == sf::Keyboard::Return ||
            event.key.code == sf::Keyboard::Space) {
            if (_selected == 0) _choice = MenuChoice::Play;
            if (_selected == 1) _showAbout = true;
            if (_selected == 2) _choice = MenuChoice::Quit;
        }
    }
    _updateColors();
}

void Menu::draw(sf::RenderWindow& win) const {
    win.draw(_title);
    if (_showAbout) {
        win.draw(_about);
        return;
    }
    for (int i = 0; i < 3; i++)
        win.draw(_items[i]);
}

void Menu::_updateColors() {
    for (int i = 0; i < 3; i++)
        _items[i].setFillColor(
            i == _selected
            ? sf::Color(70, 170, 255)
            : sf::Color(200, 200, 200)
        );
}