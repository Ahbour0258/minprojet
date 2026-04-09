// ============================================
// Cours : Classe & Objet en C++
// Cours : Constructeur & Destructeur
// Cours : Fonctions, Pointeur et Reference
// ============================================
#include "player.h"
#include <cmath>

// Constructeur
Joueur::Joueur()
    : _vitesse(0.f, 0.f),
      _etat(EtatJoueur::Vivant),
      _timerInv(0.f),
      _surSol(true),
      _accroupi(false),
      _frameAnim(0)
{
    // Corps (torse)
    _corps.setSize({32.f, 44.f});
    _corps.setOrigin(16.f, 44.f);
    _corps.setPosition(JOUEUR_X, SOL_Y);
    _corps.setFillColor(sf::Color(70, 130, 255));
    _corps.setOutlineColor(sf::Color(30, 80, 200));
    _corps.setOutlineThickness(2.f);

    // Tete
    _tete.setRadius(16.f);
    _tete.setOrigin(16.f, 16.f);
    _tete.setFillColor(sf::Color(255, 200, 130));
    _tete.setOutlineColor(sf::Color(200, 150, 80));
    _tete.setOutlineThickness(1.5f);

    // Jambes
    _jambe1.setSize({12.f, 26.f});
    _jambe1.setOrigin(6.f, 0.f);
    _jambe1.setFillColor(sf::Color(30, 80, 180));

    _jambe2.setSize({12.f, 26.f});
    _jambe2.setOrigin(6.f, 0.f);
    _jambe2.setFillColor(sf::Color(30, 80, 180));
}

// Destructeur


// Gestion des entrees clavier
void Joueur::gererEntree(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        // Saut
        if ((event.key.code == sf::Keyboard::Up ||
             event.key.code == sf::Keyboard::Space)
             && !_accroupi)
            _sauter();
        // Accroupissement
        if (event.key.code == sf::Keyboard::Down ||
            event.key.code == sf::Keyboard::S)
            _sAccroupir(true);
    }
    if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Down ||
            event.key.code == sf::Keyboard::S)
            _sAccroupir(false);
    }
}

// Mise a jour (appelee chaque frame)
void Joueur::mettreAJour(float dt) {
    // Application de la gravite
    if (!_surSol)
        _vitesse.y += GRAVITE * dt;

    _corps.move(0.f, _vitesse.y * dt);

    // Detection du sol
    if (_corps.getPosition().y >= SOL_Y) {
        _corps.setPosition(_corps.getPosition().x, SOL_Y);
        _vitesse.y = 0.f;
        _surSol    = true;
    }

    // Animation course
    _frameAnim++;

    // Mise a jour positions tete et jambes
    sf::Vector2f pos = _corps.getPosition();
    float tailleCorps = _corps.getSize().y;

    _tete.setPosition(pos.x, pos.y - tailleCorps - 12.f);

    // Animation jambes (course)
    float offsetJ = std::sin(_frameAnim * 0.18f) * 10.f;
    _jambe1.setPosition(pos.x - 8.f, pos.y);
    _jambe2.setPosition(pos.x + 8.f, pos.y - offsetJ);
    _jambe1.setSize({12.f, 26.f + offsetJ});
    _jambe2.setSize({12.f, 26.f - offsetJ});

    // Timer invincibilite
    if (_timerInv > 0.f)
        _timerInv -= dt;

    _mettreAJourCouleur();
}

// Affichage du joueur
void Joueur::afficher(sf::RenderWindow& fen) const {
    // Clignotement si invincible
    if (_timerInv > 0.f &&
        static_cast<int>(_timerInv * 8) % 2 == 0)
        return;

    fen.draw(_jambe1);
    fen.draw(_jambe2);
    fen.draw(_corps);
    fen.draw(_tete);

    // Yeux
    sf::CircleShape oeilG(3.5f), oeilD(3.5f);
    sf::CircleShape pupG(2.f),   pupD(2.f);
    sf::Vector2f posTete = _tete.getPosition();

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
    return _corps.getGlobalBounds();
}

void Joueur::recevoirChoc() {
    if (_timerInv > 0.f) return;
    if (_etat == EtatJoueur::Vivant) {
        _etat     = EtatJoueur::Blesse;
        _timerInv = 2.f;
    } else if (_etat == EtatJoueur::Blesse) {
        _etat = EtatJoueur::Mort;
    }
}

void Joueur::_sauter() {
    if (_surSol) {
        _vitesse.y = FORCE_SAUT;
        _surSol    = false;
    }
}

void Joueur::_sAccroupir(bool actif) {
    _accroupi = actif;
    if (actif) {
        _corps.setSize({32.f, 22.f});
        _corps.setOrigin(16.f, 22.f);
    } else {
        _corps.setSize({32.f, 44.f});
        _corps.setOrigin(16.f, 44.f);
    }
}

void Joueur::_mettreAJourCouleur() {
    if (_etat == EtatJoueur::Blesse)
        _corps.setFillColor(sf::Color(255, 140, 0));
    else if (_etat == EtatJoueur::Mort)
        _corps.setFillColor(sf::Color(180, 30, 30));
    else
        _corps.setFillColor(sf::Color(70, 130, 255));
}