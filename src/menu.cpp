#include "menu.h"

Menu::Menu(sf::Font& police)
    :selection(0),
    choix(ChoixMenu::Rien),
    afficherAPropos(false)
{
    // Titre du jeu
    titre.setFont(police);
    titre.setString("HIDE & RUN");
    titre.setCharacterSize(72);
    titre.setFillColor(sf::Color(70, 170, 255));
    titre.setStyle(sf::Text::Bold);
    titre.setPosition(420.f, 100.f);

    // Items du menu - tableau de chaines
    // Cours : Chaines de caracteres & Tableaux
    const std::string libelles[3] = {
        "Jouer",
        "A propos",
        "Quitter"
    };

    for (int i = 0; i < 3; i++) {
        items[i].setFont(police);
        items[i].setString(libelles[i]);
        items[i].setCharacterSize(40);
        items[i].setPosition(560.f, 270.f + i * 80.f);
    }

    // Texte A propos
    txtAPropos.setFont(police);
    txtAPropos.setCharacterSize(26);
    txtAPropos.setFillColor(sf::Color::White);
    txtAPropos.setPosition(180.f, 160.f);
    txtAPropos.setString(
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

void Menu::afficher(sf::RenderWindow& fen) const {
    fen.draw(titre);
    if (afficherAPropos) {
        fen.draw(txtAPropos);
        return;
    }
    for (int i = 0; i < 3; i++)
        fen.draw(items[i]);
}

void Menu::mettreAJourCouleurs() {
    for (int i = 0; i < 3; i++)
        items[i].setFillColor(
            i == selection
            ? sf::Color(70, 170, 255)
            : sf::Color(210, 210, 210)
        );
}