#include "player.h"
#include "marcher.h"
#include "raylib.h"

extern int SOLID_TILES[];
static float moveTimer = 0.0f;
static const float MOVE_DELAY = 0.2f;

bool verifySolidTile(int texIndex)
{
    for (int x = 0, x <= len(SOLID_TILES), x++) {
        if SOLID_TILES[x]
    }
}

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
}