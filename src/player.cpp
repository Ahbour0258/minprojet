#include "player.h"


static const float TAILLE_TETE = 28.f;
static const float LARG_CORPS = 20.f;
static const float HAUT_CORPS = 26.f;
static const float LARG_BRAS = 10.f;
static const float HAUT_BRAS = 24.f;
static const float LARG_JAMBE = 10.f;
static const float HAUT_JAMBE = 24.f;

//Couleurs peau
static const sf::Color COUL_PEAU = sf::Color(255, 213, 170); //peau beige
static const sf::Color COUL_HAUT = sf::Color(219,  92, 143); //t-shirt rose
static const sf::Color COUL_JAMBES = sf::Color( 60,  90, 170); //pantalon bleu
static const sf::Color COUL_CHEVEUX = sf::Color(120,  60,  20); //cheveux marron
static const sf::Color COUL_YEUX = sf::Color( 50,  50, 200); //yeux bleus
static const sf::Color COUL_BOUCHE = sf::Color(180,  80,  80); //bouche
static const sf::Color COUL_BLESSE = sf::Color(220, 100,  50); //blesse = orange
static const sf::Color COUL_MORT = sf::Color(100, 100, 100); //mort = gris

Joueur::Joueur()
    : vitesse(0.f, 0.f),
      etat(EtatJoueur::Vivant),
      timerInv(0.f),
      surSol(true),
      accroupi(false),
      frameAnim(0)
{
    float baseY = POS_SOL;
    //Tete
    tete.setSize({ TAILLE_TETE, TAILLE_TETE });
    tete.setFillColor(COUL_PEAU);
    tete.setOutlineColor(sf::Color(180, 140, 100));
    tete.setOutlineThickness(1.f);

    //Cheveux
    cheveux.setSize({ TAILLE_TETE, 10.f });
    cheveux.setFillColor(COUL_CHEVEUX);

    //Yeux
    oeilG.setSize({ 5.f, 5.f });
    oeilG.setFillColor(COUL_YEUX);

    oeilD.setSize({ 5.f, 5.f });
    oeilD.setFillColor(COUL_YEUX);

    //Bouche
    bouche.setSize({ 8.f, 3.f });
    bouche.setFillColor(COUL_BOUCHE);

    //Corps
    corps.setSize({ LARG_CORPS, HAUT_CORPS });
    corps.setFillColor(COUL_HAUT);
    corps.setOutlineColor(sf::Color(160, 50, 100));
    corps.setOutlineThickness(1.f);

    //Bras
    brasDroit.setSize({ LARG_BRAS, HAUT_BRAS });
    brasDroit.setFillColor(COUL_HAUT);
    brasDroit.setOutlineColor(sf::Color(160, 50, 100));
    brasDroit.setOutlineThickness(1.f);

    brasGauche.setSize({LARG_BRAS, HAUT_BRAS });
    brasGauche.setFillColor(COUL_HAUT);
    brasGauche.setOutlineColor(sf::Color(160, 50, 100));
    brasGauche.setOutlineThickness(1.f);

    //Jambes
    jambeD.setSize({LARG_JAMBE, HAUT_JAMBE });
    jambeD.setFillColor(COUL_JAMBES);
    jambeD.setOutlineColor(sf::Color(30, 50, 110));
    jambeD.setOutlineThickness(1.f);

    jambeG.setSize({LARG_JAMBE, HAUT_JAMBE });
    jambeG.setFillColor(COUL_JAMBES);
    jambeG.setOutlineColor(sf::Color(30, 50, 110));
    jambeG.setOutlineThickness(1.f);

    majPositions(baseY);
}

void Joueur::majPositions(float baseY) {
    float x = POS_X_JOUEUR;

    //Hauteur totale = jambes + corps+ tete
    float yJambes = baseY - HAUT_JAMBE;
    float yCorps = yJambes - HAUT_CORPS;
    float yBras = yCorps;
    float yTete = yCorps - TAILLE_TETE;

    // Tete centrée
    tete.setPosition(x - TAILLE_TETE / 2.f, yTete);
    cheveux.setPosition(x - TAILLE_TETE / 2.f, yTete);

    //Visage
    oeilG.setPosition(x - TAILLE_TETE / 2.f + 5.f, yTete + 12.f);
    oeilD.setPosition(x - TAILLE_TETE / 2.f + 17.f, yTete + 12.f);
    bouche.setPosition(x - TAILLE_TETE / 2.f + 9.f, yTete + 21.f);

    //Corps centre
    corps.setPosition(x - LARG_CORPS / 2.f, yCorps);

    //Bras de chaque cote du corps
    brasDroit.setPosition(x - LARG_CORPS / 2.f - LARG_BRAS - 1.f, yBras);
    brasGauche.setPosition(x + LARG_CORPS / 2.f + 1.f, yBras);

    //Jambes cote a cote
    jambeD.setPosition(x - LARG_JAMBE - 1.f, yJambes);
    jambeG.setPosition(x + 1.f, yJambes);
}

void Joueur::mettreAJour(float dt){
    if (!surSol) {
        vitesse.y += GRAVITE * dt;
    }

    float posYBase = corps.getPosition().y + HAUT_CORPS + HAUT_JAMBE;
    posYBase += vitesse.y * dt;

    if (posYBase >= POS_SOL) {
        posYBase = POS_SOL;
        vitesse.y = 0.f;
        surSol = true;
    }

    majPositions(posYBase);

    //Timer invincibilite apres choc
    if (timerInv > 0.f)
        timerInv -= dt;

    //Animation des bras et jambes pendant la course
    frameAnim = (frameAnim + 1) % 24;
    if (surSol && !accroupi) {
        float decalJ = (frameAnim < 12) ? -6.f : 6.f;
        float decalB = (frameAnim < 12) ?  5.f : -5.f;

        //Jambes alternees
        float yJambes = corps.getPosition().y + HAUT_CORPS;
        jambeD.setPosition(jambeD.getPosition().x, yJambes + (decalJ > 0 ? 0.f : 4.f));
        jambeG.setPosition(jambeG.getPosition().x, yJambes + (decalJ > 0 ? 4.f : 0.f));

        //Bras alternes
        float yBras = corps.getPosition().y;
        brasDroit.setPosition(brasDroit.getPosition().x, yBras + (decalB > 0 ? 4.f : 0.f));
        brasGauche.setPosition(brasGauche.getPosition().x, yBras + (decalB > 0 ? 0.f : 4.f));
    }
}

void Joueur::afficher(sf::RenderWindow& fen) const {
    if (timerInv > 0.f && ((int)(timerInv * 8) % 2 == 0))
        return;

    fen.draw(jambeD);
    fen.draw(jambeG);
    fen.draw(brasDroit);
    fen.draw(brasGauche);
    fen.draw(corps);
    fen.draw(tete);
    fen.draw(cheveux);
    fen.draw(oeilG);
    fen.draw(oeilD);
    fen.draw(bouche);
}

sf::FloatRect Joueur::getBornes() const {
    if (accroupi) {
        //Hitbox reduite->seulement le corps sans la tete
        return corps.getGlobalBounds();
    }
    //Hitbox normale->corps + tete
    sf::FloatRect r = corps.getGlobalBounds();
    sf::FloatRect t = tete.getGlobalBounds();
    float top = t.top;
    float bot = r.top + r.height;
    return sf::FloatRect(r.left, top, r.width, bot - top);
}

void Joueur::recevoirChoc() {
    if (timerInv > 0.f) 
    return;
    if (etat == EtatJoueur::Vivant){
        etat = EtatJoueur::Blesse;
        timerInv = 2.f;
    } else if (etat == EtatJoueur::Blesse){
        etat = EtatJoueur::Mort;
    }
    majCouleur();
}

void Joueur::majCouleur() {
    if (etat == EtatJoueur::Blesse) {
        corps.setFillColor(COUL_BLESSE);
        brasDroit.setFillColor(COUL_BLESSE);
        brasGauche.setFillColor(COUL_BLESSE);
    } else if (etat == EtatJoueur::Mort) {
        tete.setFillColor(COUL_MORT);
        corps.setFillColor(COUL_MORT);
        brasDroit.setFillColor(COUL_MORT);
        brasGauche.setFillColor(COUL_MORT);
        jambeD.setFillColor(COUL_MORT);
        jambeG.setFillColor(COUL_MORT);
        cheveux.setFillColor(sf::Color(70, 70, 70));
    }
}

void Joueur::sauter() {
    if (!surSol) 
    return;
    vitesse.y = FORCE_SAUT;
    surSol = false;
}

void Joueur::sAccroupir(bool actif) {
    if (accroupi == actif)
     return;
    accroupi = actif;
    // Quand accroupi->raccourcir les jambes
    if (accroupi) {
        jambeD.setSize({ LARG_JAMBE, HAUT_JAMBE / 2.f });
        jambeG.setSize({ LARG_JAMBE, HAUT_JAMBE / 2.f });
        tete.setFillColor(sf::Color(0, 0, 0, 0));
        tete.setOutlineColor(sf::Color(0, 0, 0, 0));
    cheveux.setFillColor(sf::Color(0, 0, 0, 0));     
    } else {
        jambeD.setSize({ LARG_JAMBE, HAUT_JAMBE });
        jambeG.setSize({ LARG_JAMBE, HAUT_JAMBE });
        tete.setFillColor(COUL_PEAU);
         tete.setOutlineColor(sf::Color(180, 140, 100));
    cheveux.setFillColor(COUL_CHEVEUX);            
    }
    majPositions(POS_SOL);
}

void Joueur::gererEntree(const sf::Event& ev) {
    if (ev.type == sf::Event::KeyPressed) {
        if (ev.key.code == sf::Keyboard::Up)
         sauter();
        if (ev.key.code == sf::Keyboard::Down) 
         sAccroupir(true);
    }
    if (ev.type == sf::Event::KeyReleased) {
        if (ev.key.code == sf::Keyboard::Down) 
           sAccroupir(false);
    }
}