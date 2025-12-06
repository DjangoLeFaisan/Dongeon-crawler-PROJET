/*systeme de combat en tour par tour , tes sorts ou tes actions possibles sont visibles tout le temps 
une fois qu'un enemie t'as vu le tour par tour commence chacun attaque et en haut une barre de priorité s'affiche pile ou face pour savoir qui commence, 
possibilité si l'enemie ne te voit pas d'attaquer en premier il va alors prendre les dégats puis lancer le combat*/ 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "game.h"

//int Battle_mode () {
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

 /*int combat (resultatordi) {
     int PV_J1 = 100 , PV_J2=80 , tour = 1 , action = 2 , mana = 100 , rage = 100 , sorts = ?, épée = 0  ; */

     
//}










