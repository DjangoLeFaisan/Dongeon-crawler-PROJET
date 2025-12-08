#include "raylib.h"

typedef struct Player {
    int gridX;
    int gridY;
    int speed;
    int texture_id;
    Texture2D texture;
} Player;