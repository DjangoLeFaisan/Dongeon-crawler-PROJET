/*systeme de combat en tour par tour , tes sorts ou tes actions possibles sont visibles tout le temps 
une fois qu'un enemie t'as vu le tour par tour commence chacun attaque et en haut une barre de priorité s'affiche pile ou face pour savoir qui commence, 
possibilité si l'enemie ne te voit pas d'attaquer en premier il va alors prendre les dégats puis lancer le combat*/ 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "game.h"


int choisirQuiCommence () {
    // Initialisation du générateur aléatoire
    int resultatordi = rand() % 2;

    // Vérification des résultats + enlever le print remplacer par le log 
    if (resultatordi == 1) {
        TraceLog(LOG_INFO,"Le monstre commence à attaquer !");
    }
    else {
        TraceLog(LOG_INFO,"Le joueur commence à attaquer !");
    }

    return resultatordi;
}

/* int bool Battle_mode () {

    
typedef struct Joueurs 
{
    int hp; 
    int maxhp; 
    int attack; 
}   
;
typedef struct Enemy 
{
    int hp; 
    int maxhp; 
    int attack; 
};

typedef struct mage
{
    int mana ; 

};
typedef struct chevalier
{
    int rage ; 
} ;


  Button attackButton = CreateButton(520, 30, 60, 25, "Attack");
}










     
//}

*/