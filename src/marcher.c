#include "player.h"
#include "marcher.h"
#include "raylib.h"
#include <math.h>

// Forward declaration du TILE_SIZE (défini dans game.h)
#define TILE_SIZE 32

static float moveTimer = 0.0f;
static const float MOVE_DELAY = 0.2f;
static const float ANIMATION_SPEED = 0.1f;

void Marcher(Player *player) 
{
    moveTimer -= GetFrameTime();
    
    int dx = 0;
    int dy = 0;

    if (IsKeyDown(KEY_W)) {       
        dy = -1;
    }
    if (IsKeyDown(KEY_S)) {
        dy = 1;
    }

    if (IsKeyDown(KEY_A)) {
        dx = -1;
    }
    if (IsKeyDown(KEY_D)) {
        dx = 1;
    }

    if ((dx != 0 || dy != 0) && moveTimer <= 0.0f) {
        player->gridX += dx;
        player->gridY += dy;
        moveTimer = MOVE_DELAY;
    }
    
    // Animer la position en pixels vers la position de grille
    float targetX = player->gridX * TILE_SIZE;
    float targetY = player->gridY * TILE_SIZE;
    
    float diffX = targetX - player->pixelX;
    float diffY = targetY - player->pixelY;
    
    // Interpolation douce vers la cible
    if (diffX != 0.0f) {
        player->pixelX += diffX * ANIMATION_SPEED;
        if (fabsf(player->pixelX - targetX) < 1.0f) {
            player->pixelX = targetX;
        }
    }
    
    if (diffY != 0.0f) {
        player->pixelY += diffY * ANIMATION_SPEED;
        if (fabsf(player->pixelY - targetY) < 1.0f) {
            player->pixelY = targetY;
        }
    }
}