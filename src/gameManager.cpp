// ============================================
// Cours : Tous les concepts C++ du module
// Activite : Implementation du gestionnaire
// ============================================
#include "gameManager.h"
#include "obstacleGround.h"
#include "obstacleAir.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <stdexcept>   // gestion des exceptions
#include <iostream>
#include <string>      // chaines de caracteres

// Constructeur - cours Constructeur & Destructeur
GestionnaireJeu::GestionnaireJeu()
    : _fenetre(sf::VideoMode(WIN_W, WIN_H), "Hide & Run"),
      _menu(_police),
      _etatJeu(EtatJeu::Menu),
      _chrono(DUREE_JEU),
      _timerSpawn(0.f),
      _prochainSpawn(1.5f),
      _offsetFond(0.f)
{
    _fenetre.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Gestion des exceptions - cours bases C++
    try {
        if (!_police.loadFromFile(CHEMIN_POLICE))
            throw std::runtime_error(
                std::string("Police introuvable : ") + CHEMIN_POLICE
            );
        if (!_textureFond.loadFromFile(CHEMIN_FOND))
            throw std::runtime_error(
                std::string("Fond introuvable : ") + CHEMIN_FOND
            );
    } catch (const std::exception& e) {
        std::cerr << "[ERREUR] " << e.what() << std::endl;
    }

    // Configuration du fond scrollant (double sprite)
    _textureFond.setRepeated(false);
    _fond.setTexture(_textureFond);
    _fond.setScale(
        (float)WIN_W / _textureFond.getSize().x,
        (float)WIN_H / _textureFond.getSize().y
    );
    _fond2 = _fond;
    _fond2.setPosition((float)WIN_W, 0.f);

    // Reinitialiser menu avec police chargee
    _menu = Menu(_police);

    // Texte chrono
    _txtChrono.setFont(_police);
    _txtChrono.setCharacterSize(30);
    _txtChrono.setFillColor(sf::Color::White);
    _txtChrono.setPosition(WIN_W / 2.f - 70.f, 14.f);

    // Texte etat joueur
    _txtEtat.setFont(_police);
    _txtEtat.setCharacterSize(24);
    _txtEtat.setPosition(18.f, 14.f);

    // Texte message final
    _txtMessage.setFont(_police);
    _txtMessage.setCharacterSize(52);
    _txtMessage.setStyle(sf::Text::Bold);

    // Sol
    _sol.setSize({(float)WIN_W, 18.f});
    _sol.setPosition(0.f, SOL_Y);
    _sol.setFillColor(sf::Color(40, 80, 20, 180));

    // Barre de progres
    _barreProgres.setSize({(float)WIN_W - 40.f, 12.f});
    _barreProgres.setPosition(20.f, WIN_H - 20.f);
    _barreProgres.setFillColor(sf::Color(0, 0, 0, 150));

    _remplissage.setSize({0.f, 12.f});
    _remplissage.setPosition(20.f, WIN_H - 20.f);
    _remplissage.setFillColor(sf::Color(50, 200, 80));
}

// Destructeur
GestionnaireJeu::~GestionnaireJeu() {
    // Les unique_ptr liberent automatiquement la memoire
    // Cours : Pointeurs et gestion memoire
}

// Boucle principale du jeu
void GestionnaireJeu::lancer() {
    sf::Clock horloge;  // Cours : bases C++ - sf::Clock

    while (_fenetre.isOpen()) {
        float dt = horloge.restart().asSeconds();
        _gererEvenements();
        if (_etatJeu == EtatJeu::EnJeu)
            _mettreAJour(dt);
        _afficher();
    }
}

// Reinitialisation complete du jeu
void GestionnaireJeu::_reinitialiser() {
    _joueur       = Joueur();
    _obstacles.clear();  // STL - vider le vecteur
    _chrono       = DUREE_JEU;
    _timerSpawn   = 0.f;
    _prochainSpawn = 1.5f;
    _offsetFond   = 0.f;
    _etatJeu      = EtatJeu::EnJeu;
}

// Gestion des evenements
void GestionnaireJeu::_gererEvenements() {
    sf::Event event;
    while (_fenetre.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            _fenetre.close();

        if (_etatJeu == EtatJeu::Menu) {
            _menu.gererEntree(event);
            ChoixMenu c = _menu.getChoix();
            if (c == ChoixMenu::Jouer)   {
                _menu.resetChoix();
                _reinitialiser();
            }
            if (c == ChoixMenu::Quitter) _fenetre.close();
        }
        else if (_etatJeu == EtatJeu::EnJeu) {
            _joueur.gererEntree(event);
        }
        else {
            // Retour menu apres game over / victoire
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Space)
                _etatJeu = EtatJeu::Menu;
        }
    }
}

// Mise a jour logique
void GestionnaireJeu::_mettreAJour(float dt) {
    // Scrolling du fond
    _offsetFond -= 220.f * dt;
    if (_offsetFond <= -(float)WIN_W)
        _offsetFond = 0.f;

    _fond.setPosition(_offsetFond, 0.f);
    _fond2.setPosition(_offsetFond + WIN_W, 0.f);

    // Mise a jour joueur
    _joueur.mettreAJour(dt);

    // Chronometre degressif
    _chrono -= dt;
    if (_chrono <= 0.f) {
        _chrono  = 0.f;
        _etatJeu = EtatJeu::GameOver;
        return;
    }
    if (_chrono <= 0.5f) {
        _etatJeu = EtatJeu::Victoire;
        return;
    }

    // Spawn d'obstacles - STL vector
    _timerSpawn += dt;
    if (_timerSpawn >= _prochainSpawn) {
        _timerSpawn    = 0.f;
        _prochainSpawn = _aleatoire(
            INTERVALLE_MIN, INTERVALLE_MAX
        );
        _genererObstacle();
    }

    // Mise a jour obstacles - boucle sur vecteur STL
    for (auto& obs : _obstacles)
        obs->deplacer(dt);

    // Supprimer obstacles hors ecran
    // Cours : STL - algorithmes
    _obstacles.erase(
        std::remove_if(
            _obstacles.begin(),
            _obstacles.end(),
            [](const std::unique_ptr<Obstacle>& o) {
                return o->horsEcran();
            }
        ),
        _obstacles.end()
    );

    _verifierCollisions();

    if (_joueur.estMort())
        _etatJeu = EtatJeu::GameOver;

    // Barre de progres
    float progres = 1.f - (_chrono / DUREE_JEU);
    _remplissage.setSize({(WIN_W - 40.f) * progres, 12.f});
}

// Generation d'obstacles aleatoires
// Cours : Pointeurs - new / unique_ptr
void GestionnaireJeu::_genererObstacle() {
    float x = WIN_W + 60.f;
    // Polymorphisme - cours Heritage
    if (std::rand() % 2 == 0)
        _obstacles.push_back(
            std::make_unique<ObstacleSol>(x)
        );
    else
        _obstacles.push_back(
            std::make_unique<ObstacleAir>(x)
        );
}

// Verification des collisions
// Cours : Transmission d'objet en argument
void GestionnaireJeu::_verifierCollisions() {
    for (const auto& obs : _obstacles) {
        if (_joueur.getBornes().intersects(obs->getBornes())) {
            _joueur.recevoirChoc();
            break;
        }
    }
}

// Dessin du fond scrollant
void GestionnaireJeu::_dessinerFond() {
    _fenetre.draw(_fond);
    _fenetre.draw(_fond2);
}

// Affichage HUD
void GestionnaireJeu::_afficherHUD() {
    // Chrono
    int sec = static_cast<int>(_chrono);
    _txtChrono.setString("Temps : " + std::to_string(sec) + "s");
    _txtChrono.setFillColor(
        sec <= 10 ? sf::Color::Red : sf::Color::White
    );
    _fenetre.draw(_txtChrono);

    // Etat du joueur
    switch (_joueur.getEtat()) {
        case EtatJoueur::Vivant:
            _txtEtat.setString("Etat : VIVANT");
            _txtEtat.setFillColor(sf::Color(80, 255, 80));
            break;
        case EtatJoueur::Blesse:
            _txtEtat.setString("Etat : BLESSE");
            _txtEtat.setFillColor(sf::Color(255, 165, 0));
            break;
        case EtatJoueur::Mort:
            _txtEtat.setString("Etat : MORT");
            _txtEtat.setFillColor(sf::Color::Red);
            break;
    }
    _fenetre.draw(_txtEtat);

    // Barre de progression
    _fenetre.draw(_barreProgres);
    _fenetre.draw(_remplissage);

    // Label bunker
    sf::Text lblBunker;
    lblBunker.setFont(_police);
    lblBunker.setString("BUNKER");
    lblBunker.setCharacterSize(13);
    lblBunker.setFillColor(sf::Color(80, 220, 80));
    lblBunker.setPosition(WIN_W - 72.f, WIN_H - 22.f);
    _fenetre.draw(lblBunker);
}

// Rendu complet
void GestionnaireJeu::_afficher() {
    _fenetre.clear(sf::Color(135, 206, 235));

    if (_etatJeu == EtatJeu::Menu) {
        _dessinerFond();
        _menu.afficher(_fenetre);
    }
    else if (_etatJeu == EtatJeu::EnJeu) {
        _dessinerFond();
        _fenetre.draw(_sol);
        for (auto& obs : _obstacles)
            obs->afficher(_fenetre);
        _joueur.afficher(_fenetre);
        _afficherHUD();
    }
    else {
        _dessinerFond();
        _fenetre.draw(_sol);
        for (auto& obs : _obstacles)
            obs->afficher(_fenetre);
        _joueur.afficher(_fenetre);
        _afficherHUD();

        // Overlay sombre
        sf::RectangleShape overlay(
            {(float)WIN_W, (float)WIN_H}
        );
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        _fenetre.draw(overlay);

        // Message final
        if (_etatJeu == EtatJeu::GameOver) {
            _txtMessage.setString("GAME OVER !");
            _txtMessage.setFillColor(sf::Color::Red);
            _txtMessage.setPosition(
                WIN_W / 2.f - 230.f,
                WIN_H / 2.f - 60.f
            );
        } else {
            _txtMessage.setString("VICTOIRE !");
            _txtMessage.setFillColor(
                sf::Color(80, 255, 80)
            );
            _txtMessage.setPosition(
                WIN_W / 2.f - 190.f,
                WIN_H / 2.f - 60.f
            );
        }
        _fenetre.draw(_txtMessage);

        sf::Text aide;
        aide.setFont(_police);
        aide.setString("Appuyez sur ESPACE pour revenir au menu");
        aide.setCharacterSize(24);
        aide.setFillColor(sf::Color(200, 200, 200));
        aide.setPosition(
            WIN_W / 2.f - 290.f,
            WIN_H / 2.f + 40.f
        );
        _fenetre.draw(aide);
    }

    _fenetre.display();
}

// Fonction generatrice de nombre aleatoire
// Cours : Fonctions en C++
float GestionnaireJeu::_aleatoire(float min, float max) {
    return min + static_cast<float>(std::rand()) /
           (static_cast<float>(RAND_MAX / (max - min)));
}