#include "marcher.h" 
#include "player.h"
#include "raylib.h"

int dx, int dy;

void marcher(Player)
{
    int dx = 0; //DX = Horizontal
    int dy = 0; //DY = Vertical

    if (IsKeyPressed[KEY_Z]) {
        dy = -1; // 1 case vers le haut
    }

    else if (IsKeyPressed[KEY_S]) {
        dy = 1; // 1 case vers le bas
    }

    else if (IsKeyPressed[KEY_Q]) {
        dx = -1; // 1 cqse vers la gauche
    }

    else if (IsKeyPressed[KEY_D]) {
        dx = 1; // 1 case vers la droite
    }
}


