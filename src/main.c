#include "raylib.h"
#include "game.h"


// Gestionnaire de texture
Texture2D gTileTextures[3];
int gTileTextureCount = 0;


int main(void)
{
<<<<<<< HEAD

    int test(choixJoueurs, resultatordi);

    const int screenWidth = BOARD_COLS * TILE_SIZE;
    const int screenHeight = BOARD_ROWS * TILE_SIZE;
=======
    const int screenWidth = 1384;
    const int screenHeight = 704;
>>>>>>> b73a9c39beb3c10b4fed85c0d70646438f55a145

    InitWindow(screenWidth, screenHeight, "Dungeon Crawler");
    SetTargetFPS(60);

    // Chargement des textures
    gTileTextures[0] = LoadTexture("assets/noir.png");
    gTileTextures[1] = LoadTexture("assets/marron.png");
    gTileTextures[2] = LoadTexture("assets/tool.png");
    gTileTextures[101] = LoadTexture("assets/sorcier_idle.png");

    gTileTextureCount = 4;
    
    Board board = {0};
    GameInit(&board);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        GameUpdate(&board, dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        GameDraw(&board);
        DrawFPS(10, 10);

        double time = GetTime();
        DrawText(TextFormat("Time : %.2f", time), 170, 10, 20, GREEN);

        EndDrawing();

        if (IsKeyPressed(KEY_I)) {
            map_editor(&board);
        }
    }

    // Libération mémoire
    for (int i = 0; i < gTileTextureCount; i++)
    {
        UnloadTexture(gTileTextures[i]);
    }

    CloseWindow();
    return 0;
}
