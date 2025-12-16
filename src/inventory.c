#include "game.h"
#include "map_editor.h"
#include "marchand.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

Texture2D money_texture = LoadTexture("assets/hud/pieces.png");

int DrawInventory(bool editor_active) {
    if (editor_active) {
        DrawTextureEx(money_texture, (Vector2){0, 0}, 0.0f, 4.0f, WHITE);
    }
    return 0;
}