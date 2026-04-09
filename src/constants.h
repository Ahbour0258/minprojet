// ============================================
// Cours : Les bases de C++ - Constantes du jeu
// Activite : Controle des parametres globaux
// ============================================
#pragma once

// -- Fenetre --
const int   WIN_W          = 1280;
const int   WIN_H          = 720;

// -- Joueur --
const float SOL_Y          = 590.f;   // position du sol
const float FORCE_SAUT     = -680.f;
const float GRAVITE        = 1350.f;
const float JOUEUR_X       = 160.f;

// -- Obstacles --
const float VITESSE_OBS    = 370.f;
const float INTERVALLE_MIN = 1.0f;
const float INTERVALLE_MAX = 2.4f;

// -- Jeu --
const float DUREE_JEU      = 60.f;   // secondes

// -- Chemins assets --
const char* const CHEMIN_POLICE     = "assets/fonts/arial.ttf";
const char* const CHEMIN_FOND       = "assets/textures/background.png";