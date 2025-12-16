#include "raylib.h"
#include "game.h"
#include "battle.h"
#include "stdlib.h"
#include "time.h"
#include "map_io.h"
#include "level_connexion.h"
#include "marchand.h"

// Gestionnaire de texture
Texture2D gTileTextures[121];
int gTileTextureCount = 0;

//Tuiles considérées comme solides
int SOLID_TILES[99] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
 26, 27, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 120};

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
    gTileTextures[29] = LoadTexture("assets/murs/mur angle bas droite.png");
    gTileTextures[30] = LoadTexture("assets/murs/mur angle bas gauche.png");
    gTileTextures[31] = LoadTexture("assets/murs/mur angle cote droite.png");
    gTileTextures[32] = LoadTexture("assets/murs/mur angle cote gauche.png");
    gTileTextures[33] = LoadTexture("assets/murs/mur angle haut droite.png");
    gTileTextures[34] = LoadTexture("assets/murs/mur angle haut gauche.png");
    gTileTextures[35] = LoadTexture("assets/murs/mur angle toit droite.png");
    gTileTextures[36] = LoadTexture("assets/murs/mur angle toit gauche.png");
    gTileTextures[37] = LoadTexture("assets/objets/caisse tonneau sacs.png");
    gTileTextures[38] = LoadTexture("assets/objets/caisse.png");
    gTileTextures[39] = LoadTexture("assets/objets/Coffre copie.png");
    gTileTextures[40] = LoadTexture("assets/objets/deux caisses petites.png");
    gTileTextures[41] = LoadTexture("assets/objets/deux caisses.png");
    gTileTextures[42] = LoadTexture("assets/objets/tonneau.png");
    gTileTextures[43] = LoadTexture("assets/objets/trois caisses.png");
    gTileTextures[44] = LoadTexture("assets/murs/mur coin bas gauche.png");
    gTileTextures[45] = LoadTexture("assets/murs/mur coin bas droite.png");


    


    gTileTextures[101] = LoadTexture("assets/personnages/sorcier/Sorcier_Idle.png");
    gTileTextures[102] = LoadTexture("assets/personnages/chevalier/chevlalier idle.png");
    gTileTextures[103] = LoadTexture("assets/personnages/chevalier/chevalierattaque/chevalierattaque1.png");
    gTileTextures[104] = LoadTexture("assets/personnages/chevalier/chevalierattaque/chevalierattaque2.png");
    gTileTextures[105] = LoadTexture("assets/personnages/chevalier/chevalierattaque/chevalierattaque3.png");
    gTileTextures[106] = LoadTexture("assets/personnages/chevalier/chevalierattaque/chevalierattaque4.png");
    gTileTextures[107] = LoadTexture("assets/personnages/chevalier/chevalierblock/chevalierblock1.png");
    gTileTextures[108] = LoadTexture("assets/personnages/chevalier/chevalierblock/chevalierblock2.png");
    gTileTextures[109] = LoadTexture("assets/personnages/orcvert/orcvertidldos.png");
    gTileTextures[110] = LoadTexture("assets/personnages/orcvert/orcvertidldroite.png");
    gTileTextures[111] = LoadTexture("assets/personnages/orcvert/orcvertidlface.png");
    gTileTextures[112] = LoadTexture("assets/personnages/orcvert/orcvertidlgauche.png");

    gTileTextures[120] = LoadTexture("assets/personnages/marchand.png");

    gTileTextureCount = 121; //Doit correspondre à l'ID max + 1
    
    Board board = {0};
    GameInit(&board);

    // Initialiser le système de combat
    extern CombatState gCombatState;
    InitCombat(&gCombatState);
    
    //Charger la première carte
    if (MapLoad(&board, "maps/couloir_defaul.map")) {
        TraceLog(LOG_INFO, "Carte chargée avec succès");
    } else {
        TraceLog(LOG_ERROR, "Erreur lors du chargement de la carte");
    }
    extern bool special_level;
    special_level = true;
    TraceLog(LOG_INFO, "Special_level = true");

    // Récupère la variable pour savoir si le joueur est dans le shop
    extern bool is_in_shop;
    // Récupère la monnaie du joueur
    extern int player_money;


    // Initialise les items duInitShopItems(); shop
    InitShopItems();
        
    while (!WindowShouldClose())
    {
       // int test (resultatordi );
        float dt = GetFrameTime();

        GameUpdate(&board, dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        GameDraw(&board);

        // Dessiner le shop et màj shop
        Vector2 mousePos = GetMousePosition();
        UpdateShopItemsHover(mousePos);  
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            HandleShopItemClick(mousePos, player_money);
        }
        BeginDrawing();
        DrawShop(is_in_shop);

        DrawFPS(400, 10);

        double time = GetTime();
        DrawText(TextFormat("Time : %.2f", time), 570, 10, 20, GREEN);

        // Ouverture de l'editeur de map
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_I)) {
            map_editor(&board);
        }
          
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_T)) 
        {
            // DrawText(TextFormat ("Le combat commence ! :  " , &resultatordi ) , 230, 25, 30, GREEN);
        }

        EndDrawing();
    }
    
    // Libération mémoire
    for (int i = 0; i < gTileTextureCount; i++)
    {
        UnloadTexture(gTileTextures[i]);
    }
    UnloadShopItems();
    CloseWindow();

    CloseWindow();
    return 0;
}
  

