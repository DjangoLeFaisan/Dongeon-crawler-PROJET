#include "raylib.h"
#include "game.h"

#define Player

typedef struct Player {
    int x;
    int y; 
    Texture2D texture; 
} Player;