#include "gameManager.h"
#include "obstacleGround.h"
#include "obstacleAir.h"
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <iostream>
#include <string>

GestionnaireJeu::GestionnaireJeu()
    : fenetre(sf::VideoMode(LARG_WIN, HAUT_WIN), "JEU CACHETTE"),
      etatJeu(EtatJeu::Menu),
      chrono(TEMPS_JEU),
      timerSpawn(0.f),
      prochainSpawn(1.5f),
      offsetFond(0.f)
{
    fenetre.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    bool policeOk = police.loadFromFile(POLICE_PATH);

    bool fondOk = texFond.loadFromFile(FOND_PATH);

    if(fondOk) {
       
        fond.setTexture(texFond);
        float scaleX = (float)LARG_WIN /(float)texFond.getSize().x;
        float scaleY = (float)HAUT_WIN / (float)texFond.getSize().y;
        fond.setScale(scaleX, scaleY);
        fond.setPosition(0.f, 0.f);
    }

    
    menu = Menu(police);
    // Texte chrono
    txtChrono.setFont(police);
    txtChrono.setCharacterSize(30);
    txtChrono.setFillColor(sf::Color(68, 8, 133));
    txtChrono.setPosition((float)LARG_WIN / 2.f - 70.f, 14.f);
    txtChrono.setStyle(sf::Text::Bold);

    // Texte etat joueur
    txtEtat.setFont(police);
    txtEtat.setCharacterSize(30);
    txtEtat.setFillColor(sf::Color(80, 255, 80));
    txtEtat.setPosition(18.f, 14.f);
    txtEtat.setStyle(sf::Text::Bold);

    // Texte message final
    txtMessage.setFont(police);
    txtMessage.setCharacterSize(52);
    txtMessage.setStyle(sf::Text::Bold);

    // Sol
    sol.setSize({ (float)LARG_WIN, 18.f });
    sol.setPosition(0.f, POS_SOL);
    sol.setFillColor(sf::Color(40, 80, 20, 180));
}

GestionnaireJeu::~GestionnaireJeu(){}

void GestionnaireJeu::lancer(){
    sf::Clock horloge;
    while (fenetre.isOpen()) {
        float dt = horloge.restart().asSeconds();
        gererEvenements();
        if (etatJeu == EtatJeu::EnJeu)
            mettreAJour(dt);
        afficher();
    }
}

void GestionnaireJeu::reinitialiser(){
    joueur = Joueur();
    obstacles.clear();
    chrono = TEMPS_JEU;
    timerSpawn = 0.f;
    prochainSpawn = 1.5f;
    offsetFond = 0.f;
    etatJeu = EtatJeu::EnJeu;
}

void GestionnaireJeu::gererEvenements() {
    sf::Event event;
    while (fenetre.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            fenetre.close();

        if (etatJeu == EtatJeu::Menu) {
            menu.gererEntree(event);
            ChoixMenu c = menu.getChoix();
            if (c == ChoixMenu::Jouer)   { menu.resetChoix(); reinitialiser(); }
            if (c == ChoixMenu::Quitter) fenetre.close();
        } else if (etatJeu == EtatJeu::EnJeu) {
            joueur.gererEntree(event);
        } else {
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Space)
                etatJeu = EtatJeu::Menu;
        }
    }
}

void GestionnaireJeu::mettreAJour(float dt) {
    // Scrolling du fond
    // Supprimer ces lignes
offsetFond -= VITESSE_OBS * dt;
if (offsetFond <= -(float)LARG_WIN)
    offsetFond = 0.f;
fond.setPosition(offsetFond, 0.f);
    joueur.mettreAJour(dt);

    chrono -= dt;
    if (chrono <= 0.f) {
        chrono = 0.f;
        etatJeu = EtatJeu::GameOver;
        return;
    }
    if (chrono <= 0.5f) {
        etatJeu = EtatJeu::Victoire;
        return;
    }

    timerSpawn += dt;
    if (timerSpawn >= prochainSpawn) {
        timerSpawn = 0.f;
        prochainSpawn = aleatoire(DELAI_MIN, DELAI_MAX);
        genererObstacle();
    }

    for (auto& obs : obstacles)
        obs->deplacer(dt);

    obstacles.erase(
        std::remove_if(
            obstacles.begin(), obstacles.end(),
            [](const std::unique_ptr<Obstacle>& o) {
                return o->horsEcran();
            }
        ),
        obstacles.end()
    );

    verifierCollisions();

    if (joueur.estMort())
        etatJeu = EtatJeu::GameOver;
}

void GestionnaireJeu::genererObstacle() {
    float x = (float)LARG_WIN + 60.f;
    if (std::rand() % 2 == 0)
        obstacles.push_back(std::make_unique<ObstacleSol>(x));
    else
        obstacles.push_back(std::make_unique<ObstacleAir>(x));
}

void GestionnaireJeu::verifierCollisions() {
    for (const auto& obs : obstacles) {
        if (joueur.getBornes().intersects(obs->getBornes())) {
            joueur.recevoirChoc();
            break;
        }
    }
}

void GestionnaireJeu::dessinerFond() {
    fond.setPosition(0.f, 0.f);
    fenetre.draw(fond);
}

void GestionnaireJeu::afficherHUD() {
    int sec = static_cast<int>(chrono);
    txtChrono.setString("Temps : " + std::to_string(sec) + "s");
    txtChrono.setFillColor(sec <= 10 ? sf::Color::Red : sf::Color::White);
    fenetre.draw(txtChrono);

    switch (joueur.getEtat()) {
        case EtatJoueur::Vivant:
            txtEtat.setString("Etat: VIVANT");
            txtEtat.setFillColor(sf::Color(80, 255, 80));
            break;
        case EtatJoueur::Blesse:
            txtEtat.setString("Etat: BLESSE");
            txtEtat.setFillColor(sf::Color(255, 165, 0));
            break;
        case EtatJoueur::Mort:
            txtEtat.setString("Etat: MORT");
            txtEtat.setFillColor(sf::Color::Red);
            break;
    }
    fenetre.draw(txtEtat);
}

void GestionnaireJeu::afficher() {
    fenetre.clear(sf::Color(135, 206, 235));

    if (etatJeu == EtatJeu::Menu) {
        dessinerFond();
        menu.afficher(fenetre);
    } else if (etatJeu == EtatJeu::EnJeu) {
        dessinerFond();
        fenetre.draw(sol);
        for (auto& obs : obstacles)
            obs->afficher(fenetre);
        joueur.afficher(fenetre);
        afficherHUD();
    } else {
        dessinerFond();
        fenetre.draw(sol);
        for (auto& obs : obstacles)
            obs->afficher(fenetre);
        joueur.afficher(fenetre);
        afficherHUD();

        // Overlay sombre
        sf::RectangleShape overlay({ (float)LARG_WIN, (float)HAUT_WIN });
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        fenetre.draw(overlay);

        if (etatJeu == EtatJeu::GameOver) {
            txtMessage.setString("GAME OVER !");
            txtMessage.setFillColor(sf::Color(219, 92, 143));
            txtMessage.setCharacterSize(100);
            txtMessage.setPosition(
                (float)LARG_WIN / 2.f - 300.f,
                (float)HAUT_WIN / 2.f - 80.f
            );
        } else {
            txtMessage.setString("VICTOIRE !");
            txtMessage.setFillColor(sf::Color(80, 255, 80));
            txtMessage.setPosition(
                (float)LARG_WIN / 2.f - 300.f,
                (float)HAUT_WIN / 2.f - 80.f
            );
        }
        fenetre.draw(txtMessage);

        sf::Text aide;
        aide.setFont(police);
        aide.setString("\"Cliquer sur ESPACE pour revenir au menu!!\"");
        aide.setCharacterSize(22);
        aide.setFillColor(sf::Color(200, 200, 200));
        aide.setPosition(
            (float)LARG_WIN / 2.f - 200.f,
            (float)HAUT_WIN / 2.f + 40.f
        );
        fenetre.draw(aide);
    }

    fenetre.display();
}

float GestionnaireJeu::aleatoire(float min, float max) {
    return min + static_cast<float>(std::rand()) /
           (static_cast<float>(RAND_MAX / (max - min)));
}