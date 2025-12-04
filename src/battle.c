/*systeme de combat en tour par tour , tes sorts ou tes actions possibles sont visibles tout le temps 
une fois qu'un enemie t'as vu le tour par tour commence chacun attaque et en haut une barre de priorité s'affiche pile ou face pour savoir qui commence, 
possibilité si l'enemie ne te voit pas d'attaquer en premier il va alors prendre les dégats puis lancer le combat */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>





int test() {
int choixJoueurs, resultatordi; /* fonctions pour stocker le choix du joueurs et le resultat*/
int lancedés(); {
 
     
       srand((unsigned)time(NULL)); // Initialisation de la génération de nombres aléatoires en fonction du temps actuel
    resultatordi = rand() % 2; // Génère un nombre aléatoire entre 0 et 1 pour simuler le côté de la pièce

    // Affichage des options pour le joueur
    printf("Choisissez un côté :\n");
    printf("0. Pile\n");
    printf("1. Face\n");
    scanf("%d", &choixJoueurs); 

    // Vérification des résultats
    if (choixJoueurs == resultatordi) { 
        if (resultatordi == 0) { 
            printf("La pièce est tombée sur Pile. Vous avez gagné !\n");
        } else { 
            printf("La pièce est tombée sur Face. Vous avez gagné !\n");
        }
    } else { 
        if (resultatordi == 0) { 
            printf("La pièce est tombée sur Pile. Vous avez perdu.\n");
        } else { // 
            printf("La pièce est tombée sur Face. Vous avez perdu.\n");
        }
    }

    return 0; 
} ;

 if (resultatordi==choixJoueurs ) {
     printf("le joueur commence à attaquer ! \n" );
   }
   else {
     printf("le monstre commence commence à attaquer ! " );
   };
   return 0;
  
}








