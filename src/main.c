#include "raylib.h"
#include "game.h"
#include "battle.h"
#include "stdlib.h"
#include "time.h"
#include "map_io.h"
#include "inventory.h"
#include "level_connexion.h"
#include "marchand.h"
#include "map_editor.h"

// Gestionnaire de texture
Texture2D money_texture;
Texture2D gTileTextures[121];
int gTileTextureCount = 0;

//Tuiles considérées comme solides
int SOLID_TILES[99] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
 26, 27, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 101, 102, 103 ,104 ,105 ,106 ,107 ,108 ,109 ,110 ,111 ,112 , 120};

// Gestionnaire de l'état de jeu
#define ETAT_EDITOR 0
int gEtatJeu = ETAT_EDITOR;

// Musique de fond globale
Music gBackgroundMusic = {0};
bool gMusicPlaying = false;

// Musique de combat globale
Music gCombatMusic = {0};
bool gCombatMusicPlaying = false;

// Musique du boss final (Etage7)
Music gBossFinalMusic = {0};
bool gBossFinalMusicPlaying = false;

// Sons globaux
Sound gWhooshSound = {0};
Sound gCutSound = {0};
Sound gBlockSound = {0};
Sound gEnemyAttackSound = {0};
Sound gDeathSound = {0};
Sound gEnemyMusic = {0};
Sound gVictoryMusic = {0};


int main(void)
{
   
    const int screenWidth = 1384;
    const int screenHeight = 704;

    InitWindow(screenWidth, screenHeight, "Dungeon Crawler");
    InitAudioDevice();

    SetTargetFPS(60);
    srand((unsigned)time(NULL));

    // Chargement des textures
    
    money_texture = LoadTexture("assets/hud/pieces.png");

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
    gTileTextures[46] = LoadTexture("assets/spawner/sol 1.png");
    gTileTextures[47] = LoadTexture("assets/sols/sol 3.png");
    gTileTextures[48] = LoadTexture("assets/sols/sol 4.png");
    gTileTextures[50] = LoadTexture("assets/sols/sol 5.png");
    gTileTextures[51] = LoadTexture("assets/sols/sol 6.png");
    gTileTextures[52] = LoadTexture("assets/sols/sol 7.png");
    gTileTextures[53] = LoadTexture("assets/sols/sol 8.png");
    gTileTextures[54] = LoadTexture("assets/sols/sol 9.png");
    gTileTextures[55] = LoadTexture("assets/sols/sol 10.png");
    gTileTextures[56] = LoadTexture("assets/sols/sol 11.png");
    gTileTextures[57] = LoadTexture("assets/sols/sol 12.png");
    gTileTextures[58] = LoadTexture("assets/sols/sol 13.png");
    gTileTextures[59] = LoadTexture("assets/sols/sol 14.png");
    gTileTextures[60] = LoadTexture("assets/sols/sol 15.png");
    gTileTextures[61] = LoadTexture("assets/sols/sol 17.png");
    gTileTextures[62] = LoadTexture("assets/sols/sol 18.png");
    gTileTextures[63] = LoadTexture("assets/sols/sol 19.png");
    gTileTextures[64] = LoadTexture("assets/sols/sol 20.png");
    gTileTextures[65] = LoadTexture("assets/sols/sol 21.png");
    gTileTextures[66] = LoadTexture("assets/sols/sol 22.png");
    gTileTextures[67] = LoadTexture("assets/sols/sol 23.png");
    gTileTextures[68] = LoadTexture("assets/sols/sol 24.png");
    gTileTextures[69] = LoadTexture("assets/sols/sol 25.png");
    gTileTextures[70] = LoadTexture("assets/eclairage/braserorose.png");
    gTileTextures[71] = LoadTexture("assets/eclairage/torche muralerose.png");

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
    gTileTextures[113] = LoadTexture("assets/personnages/BOSS/BOSSdevant.png");
    gTileTextures[114] = LoadTexture("assets/personnages/BOSS/BOSSderriere.png");
    gTileTextures[115] = LoadTexture("assets/personnages/BOSS/BOSSgauche.png");
    gTileTextures[116] = LoadTexture("assets/personnages/BOSS/BOSSdroite.png");

    gTileTextures[120] = LoadTexture("assets/personnages/marchand.png");

    gTileTextureCount = 121; //Doit correspondre à l'ID max + 1

    Board board = {0};
    GameInit(&board);

    // Initialiser le système de combat
    extern CombatState gCombatState;
    InitCombat(&gCombatState);
    
    // Charger la musique de fond (mais ne pas la jouer pour l'instant)
    gBackgroundMusic = LoadMusicStream("assets/SON/Field (Extreme Cold)  Breath of the Wild - Original Soundtrack.mp3");
    if (gBackgroundMusic.frameCount > 0) {
        gBackgroundMusic.looping = true;
        gMusicPlaying = false;
    }
    
    // Charger la musique de combat
    gCombatMusic = LoadMusicStream("assets/SON/Combat _ Lynel - The Legend of Zelda Breath of the Wild OST.mp3");
    if (gCombatMusic.frameCount > 0) {
        gCombatMusic.looping = true;
        gCombatMusicPlaying = false;
    }
    
    // Charger la musique du boss final (Etage7)
    gBossFinalMusic = LoadMusicStream("assets/SON/FFVII REMAKE_ One-Winged Angel - Rebirth (FINAL FANTASY VII REMAKE Original Soundtrack) Audio.mp3");
    if (gBossFinalMusic.frameCount > 0) {
        gBossFinalMusic.looping = true;
        gBossFinalMusicPlaying = false;
    }
    
    // Charger les sons
    gWhooshSound = LoadSound("assets/SON/SWSH_Whoosh 10 (ID 1798)_LS.wav");
    gCutSound = LoadSound("assets/SON/sf_coup_couteau_xx01.mp3");
    gBlockSound = LoadSound("assets/SON/SF-coupoing2.mp3");
    gEnemyAttackSound = LoadSound("assets/SON/sabre 9.mp3");
    gDeathSound = LoadSound("assets/SON/CREAHmn_Zombie 6 (ID 2111)_LaSonotheque.fr.wav");
    
    //Charger la première carte Etage1
    if (MapLoad(&board, "maps/couloir_defaul.map")) {
        TraceLog(LOG_INFO, "Carte chargée avec succès");
    } else {
        TraceLog(LOG_ERROR, "Erreur lors du chargement de la carte");
    }
    extern bool special_level;
    special_level = true;
    extern void SpawnEnemiesForEtage(Board *board);
    extern bool spawn_enemies_enabled;
    spawn_enemies_enabled = false;  // Désactiver le spawn sur couloir_defaul

    // Récupère la variable pour savoir si le joueur est dans le shop
    extern bool is_in_shop;
    // Récupère la monnaie du joueur
    extern int player_money;
    extern bool editor_active;

    PlayMusicStream(gBackgroundMusic);
    SetMusicVolume(gBackgroundMusic, 0.4f);
    
    // Initialise les items du shop
    InitShopItems();
        
    while (!WindowShouldClose())
    {
       // int test (resultatordi );
        float dt = GetFrameTime();
        
        // Mettre à jour la musique de fond (toujours)
        if (gBackgroundMusic.frameCount > 0) {
            UpdateMusicStream(gBackgroundMusic);
        }
        
        // Mettre à jour la musique de combat (toujours)
        if (gCombatMusic.frameCount > 0) {
            UpdateMusicStream(gCombatMusic);
        }
        
        // Mettre à jour la musique du boss final (toujours)
        if (gBossFinalMusic.frameCount > 0) {
            UpdateMusicStream(gBossFinalMusic);
        }

        GameUpdate(&board, dt);

        UpdateMusicStream(gBackgroundMusic);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        GameDraw(&board);

        // Dessiner l'inventaire
        DrawInventory(editor_active, player_money);

        // Dessiner le shop et màj shop
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            HandleShopItemClick(mousePos, &gCombatState);
        }
        DrawShop(is_in_shop);
        UpdateShopItemsHover(mousePos);  

        //DrawFPS(400, 10);

        double time = GetTime();
        // DrawText(TextFormat("Time : %.2f", time), 570, 10, 20, GREEN);

        // Ouverture de l'editeur de map
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_I)) {
            map_editor(&board);
        }

        EndDrawing();
    }
    
    // Libération mémoire
    UnloadMusicStream(gBackgroundMusic);
    UnloadMusicStream(gCombatMusic);
    UnloadMusicStream(gBossFinalMusic);
    UnloadSound(gWhooshSound);
    UnloadSound(gCutSound);
    UnloadSound(gBlockSound);
    UnloadSound(gEnemyAttackSound);
    UnloadSound(gDeathSound);
    for (int i = 0; i < gTileTextureCount; i++)
    {
        UnloadTexture(gTileTextures[i]);
    }
    UnloadShopItems();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
  

