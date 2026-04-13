#include "menu.h"

Menu::Menu(sf::Font& police)
    : selection(0),
      choix(ChoixMenu::Rien),
      afficherAPropos(false)
{
    // Fond du titre
    fondTitre.setSize({ 420.f, 78.f });
    fondTitre.setPosition(430.f, 88.f);
    fondTitre.setFillColor(sf::Color(20, 20, 50, 210));
    fondTitre.setOutlineColor(sf::Color(219, 92, 143));
    fondTitre.setOutlineThickness(2.f);

    // Titre
    titre.setFont(police);
    titre.setString("JEU CACHETTE");
    titre.setCharacterSize(40);
    titre.setFillColor(sf::Color(255, 255, 255));
    titre.setStyle(sf::Text::Bold);
    titre.setPosition(470.f, 96.f);

    // Ligne decorative
    ligne.setSize({420.f, 3.f });
    ligne.setPosition(430.f, 170.f);
    ligne.setFillColor(sf::Color(219, 92, 143));

    // Items
    const std::string libelles[3] = {"Jouer", "A propos", "Quitter" };
    for (int i = 0; i < 3; i++) {
        fondsItems[i].setSize({ 260.f, 54.f });
        fondsItems[i].setPosition(510.f, 210.f + i * 76.f);
        fondsItems[i].setOutlineThickness(2.f);

        items[i].setFont(police);
        items[i].setString(libelles[i]);
        items[i].setCharacterSize(32);
        items[i].setPosition(560.f, 218.f + i * 76.f);
    }

    // Texte A propos
    txtAPropos.setFont(police);
    txtAPropos.setCharacterSize(24);
    txtAPropos.setFillColor(sf::Color::White);
    txtAPropos.setPosition(180.f, 140.f);
    txtAPropos.setString(
        "****** JEU CACHETTE ******\n\n"
        "Commandes :\n"
        "  Fleche Haut : Sauter\n"
        "  Fleche Bas  : S'accroupir\n\n"
        "Obstacles :\n"
        "  Cactus : sauter par-dessus\n"
        "  Oiseaux : s'accroupir pour esquiver\n\n"
        "Regles :\n"
        "  1er choc = blesse\n"
        "  2e choc  = mort\n\n"
        "  ESC : Retour au menu"
    );

    mettreAJourCouleurs();
}

void Menu::gererEntree(const sf::Event& event) {
    if (afficherAPropos) {
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape)
            afficherAPropos = false;
        return;
    }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up)
            selection = (selection + 2) % 3;
        if (event.key.code == sf::Keyboard::Down)
            selection = (selection + 1) % 3;
        if (event.key.code == sf::Keyboard::Return ||
            event.key.code == sf::Keyboard::Space) {
            if (selection == 0) choix = ChoixMenu::Jouer;
            if (selection == 1) afficherAPropos = true;
            if (selection == 2) choix = ChoixMenu::Quitter;
        }
    }
    mettreAJourCouleurs();
}

void Menu::afficher(sf::RenderWindow& fen) const{
    if (afficherAPropos) {
        fen.draw(txtAPropos);
        return;
    }
    fen.draw(fondTitre);
    fen.draw(titre);
    fen.draw(ligne);
    for (int i = 0; i < 3; i++) {
        fen.draw(fondsItems[i]);
        fen.draw(items[i]);
    }
}

void Menu::mettreAJourCouleurs() {
    for (int i = 0; i < 3; i++) {
        if (i == selection) {
            fondsItems[i].setFillColor(sf::Color(219, 92, 143, 230));
            fondsItems[i].setOutlineColor(sf::Color(255, 255, 255));
            items[i].setFillColor(sf::Color(255, 255, 255));
            items[i].setStyle(sf::Text::Bold);
        } else {
            fondsItems[i].setFillColor(sf::Color(20, 20, 50, 180));
            fondsItems[i].setOutlineColor(sf::Color(120, 60, 100));
            items[i].setFillColor(sf::Color(190, 160, 180));
            items[i].setStyle(sf::Text::Regular);
        }
    }
}