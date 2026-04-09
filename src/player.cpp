#include "player.h"
#include <cmath>

// Constructeur
Joueur::Joueur()
    : vitesse(0.f, 0.f),
      etat(EtatJoueur::Vivant),
      timerInv(0.f),
      surSol(true),
      accroupi(false),
      frameAnim(0)
{
    // Corps (torse)
    corps.setSize({32.f, 44.f});
    corps.setOrigin(16.f, 44.f);
    corps.setPosition(JOUEUR_X, SOL_Y);
    corps.setFillColor(sf::Color(70, 130, 255));
    corps.setOutlineColor(sf::Color(30, 80, 200));
    corps.setOutlineThickness(2.f);

    // Tete
    tete.setRadius(16.f);
    tete.setOrigin(16.f, 16.f);
    tete.setFillColor(sf::Color(255, 200, 130));
    tete.setOutlineColor(sf::Color(200, 150, 80));
    tete.setOutlineThickness(1.5f);

    // Jambes
    jambe1.setSize({12.f, 26.f});
    jambe1.setOrigin(6.f, 0.f);
    jambe1.setFillColor(sf::Color(30, 80, 180));

    jambe2.setSize({12.f, 26.f});
    jambe2.setOrigin(6.f, 0.f);
    jambe2.setFillColor(sf::Color(30, 80, 180));
}

// Destructeur


// Gestion des entrees clavier
void Joueur::gererEntree(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        // Saut
        if ((event.key.code == sf::Keyboard::Up ||
             event.key.code == sf::Keyboard::Space)
             && !accroupi)
             sauter();
        // Accroupissement
        if (event.key.code == sf::Keyboard::Down ||
            event.key.code == sf::Keyboard::S)
            sAccroupir(true);
    }
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Down ||
            event.key.code == sf::Keyboard::S)
            sAccroupir(false);
    }
}

// Mise a jour (appelee chaque frame)
void Joueur::mettreAJour(float dt) {
    // Application de la gravite
    if (!surSol)
        vitesse.y += GRAVITE * dt;

    corps.move(0.f, vitesse.y * dt);

    // Detection du sol
    if (corps.getPosition().y >= SOL_Y) {
        corps.setPosition(corps.getPosition().x, SOL_Y);
        vitesse.y = 0.f;
        surSol = true;
    }

    // Animation course
    frameAnim++;

    // Mise a jour positions tete et jambes
    sf::Vector2f pos = corps.getPosition();
    float tailleCorps = corps.getSize().y;

    tete.setPosition(pos.x, pos.y - tailleCorps - 12.f);

    // Animation jambes (course)
    float offsetJ = std::sin(frameAnim * 0.18f) * 10.f;
    jambe1.setPosition(pos.x - 8.f, pos.y);
    jambe2.setPosition(pos.x + 8.f, pos.y - offsetJ);
    jambe1.setSize({12.f, 26.f + offsetJ});
    jambe2.setSize({12.f, 26.f - offsetJ});

    // Timer invincibilite
    if (timerInv > 0.f)
        timerInv -= dt;

    mettreAJourCouleur();
}

// Affichage du joueur
void Joueur::afficher(sf::RenderWindow& fen) const {
    // Clignotement si invincible
    if (timerInv > 0.f &&
        static_cast<int>(timerInv * 8) % 2 == 0)
        return;

    fen.draw(jambe1);
    fen.draw(jambe2);
    fen.draw(corps);
    fen.draw(tete);

    // Yeux
    sf::CircleShape oeilG(3.5f), oeilD(3.5f);
    sf::CircleShape pupG(2.f),   pupD(2.f);
    sf::Vector2f posTete = tete.getPosition();

    oeilG.setOrigin(3.5f, 3.5f);
    oeilG.setPosition(posTete.x + 5.f, posTete.y - 3.f);
    oeilG.setFillColor(sf::Color::White);

    oeilD.setOrigin(3.5f, 3.5f);
    oeilD.setPosition(posTete.x - 5.f, posTete.y - 3.f);
    oeilD.setFillColor(sf::Color::White);

    pupG.setOrigin(2.f, 2.f);
    pupG.setPosition(posTete.x + 6.f, posTete.y - 3.f);
    pupG.setFillColor(sf::Color::Black);

    pupD.setOrigin(2.f, 2.f);
    pupD.setPosition(posTete.x - 4.f, posTete.y - 3.f);
    pupD.setFillColor(sf::Color::Black);

    fen.draw(oeilG); fen.draw(oeilD);
    fen.draw(pupG);  fen.draw(pupD);
}

sf::FloatRect Joueur::getBornes() const {
    return corps.getGlobalBounds();
}

void Joueur::recevoirChoc() {
    if (timerInv > 0.f) return;
    if (etat == EtatJoueur::Vivant) {
        etat = EtatJoueur::Blesse;
        timerInv = 2.f;
    } else if (etat == EtatJoueur::Blesse) {
        etat = EtatJoueur::Mort;
    }
}

void Joueur::sauter() {
    if (surSol) {
        vitesse.y = FORCE_SAUT;
        surSol    = false;
    }
}

void Joueur::sAccroupir(bool actif) {
    accroupi = actif;
    if (actif) {
        corps.setSize({32.f, 22.f});
        corps.setOrigin(16.f, 22.f);
    } else {
        corps.setSize({32.f, 44.f});
        corps.setOrigin(16.f, 44.f);
    }
}

void Joueur::mettreAJourCouleur() {
    if (etat == EtatJoueur::Blesse)
        corps.setFillColor(sf::Color(255, 140, 0));
    else if (etat == EtatJoueur::Mort)
        corps.setFillColor(sf::Color(180, 30, 30));
    else
        corps.setFillColor(sf::Color(70, 130, 255));
}