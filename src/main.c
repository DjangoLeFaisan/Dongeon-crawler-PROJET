#include "raylib.h"
#include "game.h"
#include "stdlib.h"
#include "time.h"


// Gestionnaire de texture
Texture2D gTileTextures[29];
int gTileTextureCount = 0;



int main(void)
{

   
    const int screenWidth = 1384;
    const int screenHeight = 704;

    InitWindow(screenWidth, screenHeight, "Dungeon Crawler");
    SetTargetFPS(60);

    // Chargement des textures
    gTileTextures[0] = LoadTexture("assets/noir.png");
    gTileTextures[1] = LoadTexture("assets/marron.png");
    gTileTextures[2] = LoadTexture("assets/violet.png");

    gTileTextures[4] = LoadTexture("assets/murs/mur_bas_1.png");
    gTileTextures[5] = LoadTexture("assets/murs/mur_bas_3.png");
    gTileTextures[6] = LoadTexture("assets/murs/mur_cote_bas_droit.png");
    gTileTextures[7] = LoadTexture("assets/murs/mur_cote_bas_gauche.png");
    gTileTextures[8] = LoadTexture("assets/murs/mur_cote_bas_sol_droit.png");
    gTileTextures[9] = LoadTexture("assets/murs/mur_cote_bas_sol_gauche.png");
    gTileTextures[10] = LoadTexture("assets/murs/mur_cote_milieu_droit.png");
    gTileTextures[11] = LoadTexture("assets/murs/mur_cote_milieu_gauche.png");
    gTileTextures[12] = LoadTexture("assets/murs/mur_milieu_mod_1.png");
    gTileTextures[13] = LoadTexture("assets/murs/mur_milieu_mod_3.png");
    gTileTextures[14] = LoadTexture("assets/murs/mur_interieur.png");
    gTileTextures[15] = LoadTexture("assets/murs/toit coin gauche.png");
    gTileTextures[16] = LoadTexture("assets/murs/toit_dessous_coin_droit.png");
    gTileTextures[17] = LoadTexture("assets/murs/toit_dessous_coin_gauche.png");
    gTileTextures[18] = LoadTexture("assets/murs/toit_dessous_droite.png");
    gTileTextures[19] = LoadTexture("assets/murs/toit_dessous_gauche.png");
    gTileTextures[20] = LoadTexture("assets/murs/toit_dessus_coin_droit.png");
    gTileTextures[21] = LoadTexture("assets/murs/toit_dessus_droit.png");
    gTileTextures[22] = LoadTexture("assets/murs/toit_dessus_gauche.png");
    gTileTextures[23] = LoadTexture("assets/sols/sol 1.png");
    gTileTextures[24] = LoadTexture("assets/sols/sol 2.png");
    gTileTextures[25] = LoadTexture("assets/eclairage/brasero.png");
    gTileTextures[26] = LoadTexture("assets/eclairage/torche murale.png");


    


    gTileTextures[101] = LoadTexture("assets/personnages/sorcier/Sorcier_Idle.png");
    gTileTextures[102] = LoadTexture("assets/personnages/chevalier/chevlalier idle.png");
    

    gTileTextureCount = 29;
    
    Board board = {0};
    GameInit(&board);
    
        
    while (!WindowShouldClose())
    {
        int test (resultatordi );
        float dt = GetFrameTime();

        GameUpdate(&board, dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        GameDraw(&board);
        DrawFPS(400, 10);

        double time = GetTime();
        DrawText(TextFormat("Time : %.2f", time), 570, 10, 20, GREEN);

        EndDrawing();

        // Ouverture de l'editeur de map
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_I)) {
            map_editor(&board);
        }
        ;
          
     if (IsKeyPressed(KEY_T)) 
    {
        
      
       DrawText(TextFormat ("Le combat commence ! :  " , &resultatordi ) , 230, 25, 30, GREEN);

        EndDrawing();
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



