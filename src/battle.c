/*systeme de combat en tour par tour , tes sorts ou tes actions possibles sont visibles tout le temps 
une fois qu'un enemie t'as vu le tour par tour commence chacun attaque et en haut une barre de priorité s'affiche pile ou face pour savoir qui commence, 
possibilité si l'enemie ne te voit pas d'attaquer en premier il va alors prendre les dégats puis lancer le combat */
#include <stdio.h>



int test() {  
    int a;
    int b;
    printf("Entrez la valeur a souhaité :  \n" );  
    printf("Entrez la valeur b souhaité :  "); 
    scanf(" les valeurs rentrés sont de %lf", &a , &b);
   if (a>=b ) {
     printf("le joueur a commence  : " , &a, &b);
   }
   else {
     printf("le joueur b commence : " , &a, &b);
   };
   return 0;
}








