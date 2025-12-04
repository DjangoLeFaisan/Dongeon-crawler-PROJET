#include "raylib.h"
#include "map.h"


// Gestionnaire de texture
Texture2D gTileTextures[3];
int gTileTextureCount = 0;


int main(void)
{
    const int screenWidth = 1384;
    const int screenHeight = 704;

    InitWindow(screenWidth, screenHeight, "Dungeon Crawler");
    SetTargetFPS(60);

    // Chargement des textures
<<<<<<< HEAD
    gTileTextures[0] = LoadTexture("assets/sand.png");
    gTileTextures[1] = LoadTexture("assets/water.png");
    gTileTextures[2] = LoadTexture("assets/sorcier_idle.png");
=======
    gTileTextures[0] = LoadTexture("assets/noir.png");
    gTileTextures[1] = LoadTexture("assets/marron.png");
    gTileTextures[2] = LoadTexture("assets/tool.png");
>>>>>>> 2b42345e8b69d85d108ed5613629e629051ae073
    gTileTextureCount = 3;
    
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
    }

    // Libération mémoire
    for (int i = 0; i < gTileTextureCount; i++)
    {
        UnloadTexture(gTileTextures[i]);
    }

    CloseWindow();
    return 0;
}
