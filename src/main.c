#include "raylib.h"
#include "game.h"
#include "stdlib.h"
#include "time.h"


// Gestionnaire de texture
Texture2D gTileTextures[30];
int gTileTextureCount = 0;

// Gestionnaire de l'état de jeu
#define ETAT_EDITOR 0
int gEtatJeu = ETAT_EDITOR;

//Gestionnaire de l'état de combat (plus tard)


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

    gTileTextures[4] = LoadTexture("assets/murs/mur bas 1.png");
    gTileTextures[5] = LoadTexture("assets/murs/mur bas 3.png");
    gTileTextures[6] = LoadTexture("assets/murs/mur cote bas droit_.png");
    gTileTextures[7] = LoadTexture("assets/murs/mur cote bas gauche_.png");
    gTileTextures[8] = LoadTexture("assets/murs/mur cote bas sol droit.png");
    gTileTextures[9] = LoadTexture("assets/murs/mur cote bas sol gauche.png");
    gTileTextures[10] = LoadTexture("assets/murs/mur cote millieu droit.png");
    gTileTextures[11] = LoadTexture("assets/murs/mur cote millieu gauche.png");
    gTileTextures[12] = LoadTexture("assets/murs/mur millieu mod 1.png");
    gTileTextures[13] = LoadTexture("assets/murs/mur millieu mod 3.png");
    gTileTextures[14] = LoadTexture("assets/murs/mur_interieur.png");
    gTileTextures[15] = LoadTexture("assets/murs/toit coin gauche.png");
    gTileTextures[16] = LoadTexture("assets/murs/toit dessous coin droit.png");
    gTileTextures[17] = LoadTexture("assets/murs/toit dessous coin gauche.png");
    gTileTextures[18] = LoadTexture("assets/murs/toit dessous droite.png");
    gTileTextures[19] = LoadTexture("assets/murs/toit dessous gauche.png");
    gTileTextures[20] = LoadTexture("assets/murs/toit dessus coin droit.png");
    gTileTextures[21] = LoadTexture("assets/murs/toit dessus droit.png");
    gTileTextures[22] = LoadTexture("assets/murs/toit dessus gauche.png");
    gTileTextures[23] = LoadTexture("assets/sols/sol 1.png");
    gTileTextures[24] = LoadTexture("assets/sols/sol 2.png");
    gTileTextures[25] = LoadTexture("assets/eclairage/brasero.png");
    gTileTextures[26] = LoadTexture("assets/eclairage/torche murale.png");
    gTileTextures[27] = LoadTexture("assets/sprites portes/porte close.png");
    gTileTextures[28] = LoadTexture("assets/sprites portes/porte open.png");


    


    gTileTextures[101] = LoadTexture("assets/personnages/sorcier/Sorcier_Idle.png");
    gTileTextures[102] = LoadTexture("assets/personnages/chevalier/chevlalier idle.png");
    

    gTileTextureCount = 30;
    
    Board board = {0};
    GameInit(&board);
    
        
    while (!WindowShouldClose())
    {
   
        float dt = GetFrameTime();

        GameUpdate(&board, dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        GameDraw(&board);
        DrawFPS(400, 10);

        double time = GetTime();
        DrawText(TextFormat("Time : %.2f", time), 570, 10, 20, GREEN);

        // Ouverture de l'editeur de map
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_I)) {
            map_editor(&board);
        }

          
        /*if (IsKeyPressed(KEY_T)) 
        {
            
        
            DrawText(TextFormat ("Le combat commence ! :  ") , 230, 25, 30, GREEN);


        }

        EndDrawing(); */
    }
    

    // Libération mémoire
    for (int i = 0; i < gTileTextureCount; i++)
    {
        UnloadTexture(gTileTextures[i]);
    }

    CloseWindow();
    return 0;
}  



