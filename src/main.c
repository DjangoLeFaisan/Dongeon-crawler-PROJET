#include "raylib.h"
#include "game.h"


// Gestionnaire de texture
Texture2D gTileTextures[3];
int gTileTextureCount = 0;


int main(void)
{

    /*int test(choixJoueurs, resultatordi);*/

    const int screenWidth = 1384;
    const int screenHeight = 704;

    InitWindow(screenWidth, screenHeight, "Dungeon Crawler");
    SetTargetFPS(60);

    // Chargement des textures
    gTileTextures[0] = LoadTexture("assets/noir.png");
    gTileTextures[1] = LoadTexture("assets/marron.png");
    gTileTextures[2] = LoadTexture("assets/tool.png");
    gTileTextures[3] = LoadTexture("assets/murs/mur bas 1 copie.png");
    gTileTextures[4] = LoadTexture("assets/murs/mur bas 3 copie.png");
    gTileTextures[5] = LoadTexture("assets/murs/mur coté bas droit_.png");
    gTileTextures[6] = LoadTexture("assets/murs/mur coté bas gauche_.png");
    gTileTextures[7] = LoadTexture("assets/murs/mur coté bas sol droit.png");
    gTileTextures[8] = LoadTexture("assets/murs/mur coté bas sol gauche.png");
    gTileTextures[9] = LoadTexture("assets/murs/mur coté millieu droit.png");
    gTileTextures[10] = LoadTexture("assets/murs/mur coté millieu gauche.png");
    gTileTextures[11] = LoadTexture("assets/murs/mur millieu mod 1.png");
    gTileTextures[12] = LoadTexture("assets/murs/mur millieu mod 3.png");
    gTileTextures[13] = LoadTexture("assets/murs/Noir.png");
    gTileTextures[14] = LoadTexture("assets/murs/toit coin gauche.png");
    gTileTextures[15] = LoadTexture("assets/murs/toit dessous coin droit.png");
    gTileTextures[16] = LoadTexture("assets/murs/toit dessous coin gauche.png");
    gTileTextures[17] = LoadTexture("assets/murs/toit dessous droite.png");
    gTileTextures[18] = LoadTexture("assets/murs/toit dessous gauche.png");
    gTileTextures[19] = LoadTexture("assets/murs/toit dessus coin droit.png");
    gTileTextures[20] = LoadTexture("assets/murs/toit dessus droit.png");
    gTileTextures[21] = LoadTexture("assets/murs/toit dessus gauche.png");


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

        // Ouverture de l'editeur de map
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
