#pragma once
#include <SFML/Graphics.hpp>

enum class MenuChoice { None, Play, About, Quit };

class Menu {
public:
    explicit Menu(sf::Font& font);

    void       handleInput(const sf::Event& event);
    void       draw(sf::RenderWindow& win) const;
    MenuChoice getChoice()  const { return _choice; }
    void       resetChoice()     { _choice = MenuChoice::None; }

private:
    sf::Text   _title;
    sf::Text   _items[3];
    sf::Text   _about;
    int        _selected;
    MenuChoice _choice;
    bool       _showAbout;

    void _updateColors();
};