#include "raylib.h"

typedef struct Player {
    int gridX;
    int gridY;
    float pixelX;       // Position en pixels pour l'animation
    float pixelY;       // Position en pixels pour l'animation
    int speed;
    int texture_id;
    Texture2D texture;
    int direction;      // 1 = droite, -1 = gauche
} Player;