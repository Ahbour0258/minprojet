#pragma once

//Fenêtre
const int LARG_WIN = 1280;
const int HAUT_WIN= 720;
//Joueur
const float POS_SOL = 590.f;
const float FORCE_SAUT = -680.f;
const float GRAVITE = 1350.f;
const float POS_X_JOUEUR = 160.f;
//Obstacles
const float VITESSE_OBS = 370.f;
const float DELAI_MIN = 1.0f;
const float DELAI_MAX = 2.4f;
//Jeu
const float TEMPS_JEU = 60.f;
//Assets
const char* const POLICE_PATH = "assets/fonts/arial.ttf"; //Police d'écriture
const char* const FOND_PATH= "assets/textures/background.png"; //L'image background de notre jeu