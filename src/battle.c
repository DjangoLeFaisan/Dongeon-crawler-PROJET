/*systeme de combat en tour par tour , tes sorts ou tes actions possibles sont visibles tout le temps 
une fois qu'un enemie t'as vu le tour par tour commence chacun attaque et en haut une barre de priorité s'affiche pile ou face pour savoir qui commence, 
possibilité si l'enemie ne te voit pas d'attaquer en premier il va alors prendre les dégats puis lancer le combat*/ 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>





int test(int choixJoueurs, int resultatordi) {


     // Affichage des options
    printf("Choisissez un côté :\n");
    printf("0. Pile\n");
    printf("1. Face\n");
    scanf("%d", &choixJoueurs);
    
    // Initialisation du générateur aléatoire
    int GetRandomValue(int min, int max);;
    resultatordi = rand() % 2;

    // Vérification des résultats + enlever le print remplacer par le log 
    if (choixJoueurs == resultatordi) {
        if (resultatordi == 0) {
            printf("La pièce est tombée sur Pile. Vous avez gagné !\n");
        } else {
            printf("La pièce est tombée sur Face. Vous avez gagné !\n");
        }
        printf("Le joueur commence à attaquer !\n");
    } else {
        if (resultatordi == 0) {
            printf("La pièce est tombée sur Pile. Vous avez perdu.\n");
        } else {
            printf("La pièce est tombée sur Face. Vous avez perdu.\n");
        }
        printf("Le monstre commence à attaquer !\n");
    }

    return 0;
}

 int combat (resultatordi) {
     int PV_J1 = 100 , PV_J2=80 , tour = 1 , action = 2 , mana = 100 , rage = 100 , sorts = ?, épée = 0  ; 
    
    
    
    
    
    
    
    }
     











