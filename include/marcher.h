#ifndef MARCHER_H
#define MARCHER_H

#include "game.h"

typedef struct Player Player;

// Vérifier si un index de tuile est solide (collision)
bool verifySolidTile(int tileIndex);

// Récupérer l'index de tuile aux coordonnées données
int GetTileAtGridPos(const Board *board, int gridX, int gridY);

// Gérer le mouvement du joueur
void Marcher(Player *player, Board *board);

#endif // MARCHER_H