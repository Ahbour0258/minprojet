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
    : fenetre(sf::VideoMode(WIN_W, WIN_H), "Hide & Run"),
      menu(police),
      etatJeu(EtatJeu::Menu),
      chrono(DUREE_JEU),
      timerSpawn(0.f),
      prochainSpawn(1.5f),
      offsetFond(0.f)
{
    fenetre.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Gestion des exceptions - cours bases C++
    try {
        if (!police.loadFromFile(CHEMIN_POLICE))
            throw std::runtime_error(
                std::string("Police introuvable : ") + CHEMIN_POLICE
            );
        if (!textureFond.loadFromFile(CHEMIN_FOND))
            throw std::runtime_error(
                std::string("Fond introuvable : ") + CHEMIN_FOND
            );
    } catch (const std::exception& e) {
        std::cerr << "[ERREUR] " << e.what() << std::endl;
    }

    // Configuration du fond scrollant (double sprite)
    textureFond.setRepeated(false);
    fond.setTexture(textureFond);
    fond.setScale(
        (float)WIN_W / textureFond.getSize().x,
        (float)WIN_H / textureFond.getSize().y
    );
    fond2 = fond;
    fond2.setPosition((float)WIN_W, 0.f);

    // Reinitialiser menu avec police chargee
    menu = Menu(police);

    // Texte chrono
    txtChrono.setFont(police);
    txtChrono.setCharacterSize(30);
    txtChrono.setFillColor(sf::Color::White);
    txtChrono.setPosition(WIN_W / 2.f - 70.f, 14.f);

    // Texte etat joueur
    txtEtat.setFont(police);
    txtEtat.setCharacterSize(24);
    txtEtat.setPosition(18.f, 14.f);

    // Texte message final
    txtMessage.setFont(police);
    txtMessage.setCharacterSize(52);
    txtMessage.setStyle(sf::Text::Bold);

    // Sol
    sol.setSize({(float)WIN_W, 18.f});
    sol.setPosition(0.f, SOL_Y);
    sol.setFillColor(sf::Color(40, 80, 20, 180));

    // Barre de progres
    barreProgres.setSize({(float)WIN_W - 40.f, 12.f});
    barreProgres.setPosition(20.f, WIN_H - 20.f);
    barreProgres.setFillColor(sf::Color(0, 0, 0, 150));

    remplissage.setSize({0.f, 12.f});
    remplissage.setPosition(20.f, WIN_H - 20.f);
    remplissage.setFillColor(sf::Color(50, 200, 80));
}

// Destructeur
GestionnaireJeu::~GestionnaireJeu() {
    // Les unique_ptr liberent automatiquement la memoire
    // Cours : Pointeurs et gestion memoire
}

// Boucle principale du jeu
void GestionnaireJeu::lancer() {
    sf::Clock horloge;  // Cours : bases C++ - sf::Clock

    while (fenetre.isOpen()) {
        float dt = horloge.restart().asSeconds();
        gererEvenements();
        if (etatJeu == EtatJeu::EnJeu)
            mettreAJour(dt);
        afficher();
    }
}

// Reinitialisation complete du jeu
void GestionnaireJeu::reinitialiser() {
    joueur       = Joueur();
    obstacles.clear();  // STL - vider le vecteur
    chrono       = DUREE_JEU;
    timerSpawn   = 0.f;
    prochainSpawn = 1.5f;
    offsetFond   = 0.f;
    etatJeu      = EtatJeu::EnJeu;
}

// Gestion des evenements
void GestionnaireJeu::gererEvenements() {
    sf::Event event;
    while (fenetre.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            fenetre.close();

        if (etatJeu == EtatJeu::Menu) {
            menu.gererEntree(event);
            ChoixMenu c = menu.getChoix();
            if (c == ChoixMenu::Jouer)   {
                menu.resetChoix();
                reinitialiser();
            }
            if (c == ChoixMenu::Quitter) fenetre.close();
        }
        else if (etatJeu == EtatJeu::EnJeu) {
            joueur.gererEntree(event);
        }
        else {
            // Retour menu apres game over / victoire
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Space)
                etatJeu = EtatJeu::Menu;
        }
    }
}

// Mise a jour logique
void GestionnaireJeu::mettreAJour(float dt) {
    // Scrolling du fond
    offsetFond -= 220.f * dt;
    if (offsetFond <= -(float)WIN_W)
        offsetFond = 0.f;

    fond.setPosition(offsetFond, 0.f);
    fond2.setPosition(offsetFond + WIN_W, 0.f);

    // Mise a jour joueur
    joueur.mettreAJour(dt);

    // Chronometre degressif
    chrono -= dt;
    if (chrono <= 0.f) {
        chrono  = 0.f;
        etatJeu = EtatJeu::GameOver;
        return;
    }
    if (chrono <= 0.5f) {
        etatJeu = EtatJeu::Victoire;
        return;
    }

    // Spawn d'obstacles - STL vector
    timerSpawn += dt;
    if (timerSpawn >= prochainSpawn) {
        timerSpawn    = 0.f;
        prochainSpawn = aleatoire(
            INTERVALLE_MIN, INTERVALLE_MAX
        );
        genererObstacle();
    }

    // Mise a jour obstacles - boucle sur vecteur STL
    for (auto& obs : obstacles)
        obs->deplacer(dt);

    // Supprimer obstacles hors ecran
    // Cours : STL - algorithmes
    obstacles.erase(
        std::remove_if(
            obstacles.begin(),
            obstacles.end(),
            [](const std::unique_ptr<Obstacle>& o) {
                return o->horsEcran();
            }
        ),
        obstacles.end()
    );

    verifierCollisions();

    if (joueur.estMort())
        etatJeu = EtatJeu::GameOver;

    // Barre de progres
    float progres = 1.f - (chrono / DUREE_JEU);
    remplissage.setSize({(WIN_W - 40.f) * progres, 12.f});
}

// Generation d'obstacles aleatoires
// Cours : Pointeurs - new / unique_ptr
void GestionnaireJeu::genererObstacle() {
    float x = WIN_W + 60.f;
    // Polymorphisme - cours Heritage
    if (std::rand() % 2 == 0)
        obstacles.push_back(
            std::make_unique<ObstacleSol>(x)
        );
    else
        obstacles.push_back(
            std::make_unique<ObstacleAir>(x)
        );
}

// Verification des collisions
// Cours : Transmission d'objet en argument
void GestionnaireJeu::verifierCollisions() {
    for (const auto& obs : obstacles) {
        if (joueur.getBornes().intersects(obs->getBornes())) {
            joueur.recevoirChoc();
            break;
        }
    }
}

// Dessin du fond scrollant
void GestionnaireJeu::dessinerFond() {
    fenetre.draw(fond);
    fenetre.draw(fond2);
}

// Affichage HUD
void GestionnaireJeu::afficherHUD() {
    // Chrono
    int sec = static_cast<int>(chrono);
    txtChrono.setString("Temps : " + std::to_string(sec) + "s");
    txtChrono.setFillColor(
        sec <= 10 ? sf::Color::Red : sf::Color::White
    );
    fenetre.draw(txtChrono);

    // Etat du joueur
    switch (joueur.getEtat()) {
        case EtatJoueur::Vivant:
            txtEtat.setString("Etat : VIVANT");
            txtEtat.setFillColor(sf::Color(80, 255, 80));
            break;
        case EtatJoueur::Blesse:
            txtEtat.setString("Etat : BLESSE");
            txtEtat.setFillColor(sf::Color(255, 165, 0));
            break;
        case EtatJoueur::Mort:
            txtEtat.setString("Etat : MORT");
            txtEtat.setFillColor(sf::Color::Red);
            break;
    }
    fenetre.draw(txtEtat);

    // Barre de progression
    fenetre.draw(barreProgres);
    fenetre.draw(remplissage);

    // Label bunker
    sf::Text lblBunker;
    lblBunker.setFont(police);
    lblBunker.setString("BUNKER");
    lblBunker.setCharacterSize(13);
    lblBunker.setFillColor(sf::Color(80, 220, 80));
    lblBunker.setPosition(WIN_W - 72.f, WIN_H - 22.f);
    fenetre.draw(lblBunker);
}

// Rendu complet
void GestionnaireJeu::afficher() {
    fenetre.clear(sf::Color(135, 206, 235));

    if (etatJeu == EtatJeu::Menu) {
        dessinerFond();
        menu.afficher(fenetre);
    }
    else if (etatJeu == EtatJeu::EnJeu) {
        dessinerFond();
        fenetre.draw(sol);
        for (auto& obs : obstacles)
            obs->afficher(fenetre);
        joueur.afficher(fenetre);
        afficherHUD();
    }
    else {
        dessinerFond();
        fenetre.draw(sol);
        for (auto& obs : obstacles)
            obs->afficher(fenetre);
        joueur.afficher(fenetre);
        afficherHUD();

        // Overlay sombre
        sf::RectangleShape overlay(
            {(float)WIN_W, (float)WIN_H}
        );
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        fenetre.draw(overlay);

        // Message final
        if (etatJeu == EtatJeu::GameOver) {
            txtMessage.setString("GAME OVER !");
            txtMessage.setFillColor(sf::Color::Red);
            txtMessage.setPosition(
                WIN_W / 2.f - 230.f,
                WIN_H / 2.f - 60.f
            );
        } else {
            txtMessage.setString("VICTOIRE !");
            txtMessage.setFillColor(
                sf::Color(80, 255, 80)
            );
            txtMessage.setPosition(
                WIN_W / 2.f - 190.f,
                WIN_H / 2.f - 60.f
            );
        }
        fenetre.draw(txtMessage);

        sf::Text aide;
        aide.setFont(police);
        aide.setString("Appuyez sur ESPACE pour revenir au menu");
        aide.setCharacterSize(24);
        aide.setFillColor(sf::Color(200, 200, 200));
        aide.setPosition(
            WIN_W / 2.f - 290.f,
            WIN_H / 2.f + 40.f
        );
        fenetre.draw(aide);
    }

    fenetre.display();
}

// Fonction generatrice de nombre aleatoire
// Cours : Fonctions en C++
float GestionnaireJeu::aleatoire(float min, float max) {
    return min + static_cast<float>(std::rand()) /
           (static_cast<float>(RAND_MAX / (max - min)));
}