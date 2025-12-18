#include "raylib.h"
#include "game.h"
#include "map_io.h"
#include "marcher.h"
#include "battle.h"
#include "enemy.h"
#include "level_connexion.h"
#include "marchand.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PLAYABLE_ZONE_WIDTH 1088
#define PLAYABLE_ZONE_HEIGTH 704

extern Texture2D gTileTextures[];
extern int gTileTextureCount;
extern bool editor_active;
extern bool special_level;
extern int current_level;
extern ShopItem shop_items[MAX_SHOP_ITEMS];

extern int player_money;
extern double force_modifier;
extern double defense_modifier;
extern double speed_modifier;
extern double range_modifier;
extern double attack_speed_modifier;
extern double rage_modifier;
extern int avarice_modifier;
extern int ennemies_to_kill;
extern int ennemies_killed;

bool spawn_enemies_enabled = false;  // Variable globale pour contrôler le spawn des ennemis

int objectIndex = 4;
static int lastPreviewX = -1;
static int lastPreviewY = -1;
static bool game_over = false;
static float game_over_timer = 0.0f;

// Gestion du nom de carte
#define MAX_MAP_NAME_LENGTH 64
static char currentMapName[MAX_MAP_NAME_LENGTH] = "map_default";
static bool isNamingMap = false;
static int namingCursorPos = 0;

// Construit le chemin complet du fichier
static void GetMapFilepath(const char *mapName, char *filepath, int maxLen)
{
    snprintf(filepath, maxLen, "maps/%s.map", mapName);
}


// ******************************************
//

void TileClear(Tile *t)
{
    t->layerCount = 0;
    for (int i = 0; i < MAX_LAYERS; i++)
    {
        t->layers[i] = -1;
    }
}

bool TilePush(Tile *t, int texIndex)
{
    if (t->layerCount >= MAX_LAYERS)
        return false;
    t->layers[t->layerCount++] = texIndex;
    return true;
}

int TilePop(Tile *t)
{
    if (t->layerCount <= 1)
        return -1;
    int tex = t->layers[--t->layerCount];
    t->layers[t->layerCount] = -1;
    return tex;
}

void GameInit(Board *board)
{
    board->player.gridX = 0;
    board->player.gridY = 0;
    board->player.pixelX = 5 * TILE_SIZE;
    board->player.pixelY = 5 * TILE_SIZE;
    board->player.speed = 1;
    board->player.texture_id = 102;
    board->player.lastDirection = 1;  // Direction initiale : droite
    board->enemyCount = 0;
    ResetEnemies(board);
    
    for (int y = 0; y < BOARD_ROWS; y++)
    {
        for (int x = 0; x < BOARD_COLS; x++)
        {
            Tile *t = &board->tiles[y][x];
            TileClear(t);

            int groundIndex = 0;

            if (x < 34)
                groundIndex = 0;
            else 
                groundIndex = 1;
            
            TilePush(t, groundIndex);
        }
    }
}


void GameUpdate(Board *board, float dt)
{
    // Vérifier si le joueur est mort
    extern CombatState gCombatState;
    if (gCombatState.knight.hp <= 0 && !game_over) {
        game_over = true;
        game_over_timer = 0.0f;
        TraceLog(LOG_INFO, "GAME OVER! Le joueur est mort!");
    }
    
    // Si game over, gérer les boutons
    if (game_over) {
        Vector2 mousePos = GetMousePosition();
        
        // Définir les rectangles des boutons
        Rectangle btnReessaie = {((PLAYABLE_ZONE_WIDTH / 2) - 70) - 100, 400, 140, 60};
        Rectangle btnAbandoner = {((PLAYABLE_ZONE_WIDTH / 2) - 70) + 100, 400, 140, 60};
        
        // Vérifier si les boutons sont cliqués
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mousePos, btnReessaie)) {
                // Réinitialiser complètement le jeu
                game_over = false;
                game_over_timer = 0.0f;
                
                // Réinitialiser le joueur
                GameInit(board);
                
                // Réinitialiser le combat
                InitCombat(&gCombatState);
                
                // Réinitialiser les ennemis
                ResetEnemies(board);
                
                // Réinitialiser le niveau
                current_level = 1;
                special_level = true;

                // Réinitialiser les stats du joueur et le shop
                player_money = 0;
                force_modifier = 1.0;
                defense_modifier = 1.0;
                speed_modifier = 1.0;
                range_modifier = 1.0;
                attack_speed_modifier = 1.0;
                rage_modifier = 1.0;
                avarice_modifier = 1;
                for (int i = 0; i < MAX_SHOP_ITEMS; i++) {
                    shop_items[i].currentStack = 0;
                }

                
                // Désactiver le spawn pour couloir_defaul
                extern bool spawn_enemies_enabled;
                spawn_enemies_enabled = false;
                
                // Recharger la première carte (couloir sans ennemis)
                if (MapLoad(board, "maps/couloir_defaul.map")) {
                    TraceLog(LOG_INFO, "Carte chargée avec succès: maps/couloir_defaul.map");
                    ennemies_killed = 0;
                    ennemies_to_kill = 0;
                } else {
                    TraceLog(LOG_ERROR, "Erreur lors du chargement de la carte: maps/couloir_defaul.map");
                }

            } else if (CheckCollisionPointRec(mousePos, btnAbandoner)) {
                // Quitter le jeu
                exit(0);
            }
        }
        return;  // Ne pas mettre à jour le jeu si game over
    }
    
    // Gestion du joueur
    Marcher(&board->player, board);
    
    // Mise à jour du combat overlay
    UpdateCombat(&gCombatState, dt);
    UpdateAttackHitboxesFromPlayer(&gCombatState, board->player.pixelX, board->player.pixelY);
    UpdateEnemies(board, dt, &gCombatState);
    UpdateProgressiveSpawn(board, dt);
    
    Vector2 m = GetMousePosition();
    int tileX = (int)(m.x) / TILE_SIZE;
    int tileY = (int)(m.y) / TILE_SIZE;

    // Gestion de la tuile factice (éditeur de carte)
    if (tileX != lastPreviewX || tileY != lastPreviewY) {

        // Retire la tuile factice de l'ancienne position
        if (lastPreviewX >= 0 && lastPreviewX < BOARD_COLS && lastPreviewY >= 0 && lastPreviewY < BOARD_ROWS) {
            Tile *oldTile = &board->tiles[lastPreviewY][lastPreviewX];
            TilePop(oldTile);
        }
        
        // Ajoute la tuile factice à la nouvelle position
        if ((tileX <= 33) && editor_active) {
            Tile *newTile = &board->tiles[tileY][tileX];
            TilePush(newTile, objectIndex);
            lastPreviewX = tileX;
            lastPreviewY = tileY;
        } else {
            lastPreviewX = -1;
            lastPreviewY = -1;
        }
    }
    
    // Gestion des intéractions en mode éditeur de carte 
    if (editor_active == true) {

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (tileX <= 33)) //Placer une tuile
        {   
            TraceLog(LOG_INFO,
                "Tuile plac�e au coordonn�es x=%.1f y=%.1f � la tuile correspondante : (%d, %d)",
                m.x, m.y, tileX, tileY);
                
                Tile *t = &board->tiles[tileY][tileX];
                TilePush(t, objectIndex);
        
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && (tileX <= 33)) // Enlever la tuile la plus haute
        {
            TraceLog(LOG_INFO,
                "Tuile enlev�e au coordonn�es x=%.1f y=%.1f � la tuile correspondante : (%d, %d)",
                m.x, m.y, tileX, tileY);
                
                Tile *t = &board->tiles[tileY][tileX];
                TilePop(t);

        } else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) // Sélectionner la tuile la plus haute
        {
            TraceLog(LOG_INFO,
                "Type de tuile s�lectionn�e au coordonn�es x=%.1f y=%.1f � la tuile correspondante : (%d, %d)",
                m.x, m.y, tileX, tileY);
                
                Tile *t = &board->tiles[tileY][tileX];

                int textureIndex = 0;
                if (tileX <= 33) {
                    textureIndex = t->layers[t->layerCount - 2];
                } else {
                    textureIndex = t->layers[t->layerCount - 1];
                }

                if (textureIndex >= 4) {
                    objectIndex = textureIndex;
                }
        }

        // Gestion du nom de carte (Tab pour éditer)
        if (IsKeyPressed(KEY_TAB)) {
            isNamingMap = !isNamingMap;
            namingCursorPos = strlen(currentMapName);
        }

        // Edition du nom de carte
        if (isNamingMap) {
            // Traiter les touches de caractères
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32 && key < 127) && namingCursorPos < MAX_MAP_NAME_LENGTH - 1) {
                    currentMapName[namingCursorPos++] = (char)key;
                    currentMapName[namingCursorPos] = '\0';
                }
                key = GetCharPressed();
            }

            // Backspace pour supprimer
            if (IsKeyPressed(KEY_BACKSPACE) && namingCursorPos > 0) {
                currentMapName[--namingCursorPos] = '\0';
            }

            // Enter pour confirmer
            if (IsKeyPressed(KEY_ENTER)) {
                isNamingMap = false;
            }
        }

        // Sauvegarde (Ctrl+S)
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
            char filepath[256];
            GetMapFilepath(currentMapName, filepath, sizeof(filepath));
            if (MapSave(board, filepath)) {
                TraceLog(LOG_INFO, "Map saved to %s", filepath);
            } else {
                TraceLog(LOG_ERROR, "Failed to save map");
            }
        }

        // Chargement (Ctrl+L)
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L)) {
            char filepath[256];
            GetMapFilepath(currentMapName, filepath, sizeof(filepath));
            if (MapLoad(board, filepath)) {
                TraceLog(LOG_INFO, "Map loaded from %s", filepath);
            } else {
                TraceLog(LOG_ERROR, "Failed to load map");
            }
        }
    }

    // Gestion des entrées clavier
    if (IsKeyPressed(KEY_SPACE))
    {
        TraceLog(LOG_INFO, "SPACE pressed in GameUpdate");
    }
}

void GameDraw(const Board *board)
{
    static bool logged = false;
    if (!logged) {
        TraceLog(LOG_INFO, "GameDraw called! BOARD_ROWS=%d, BOARD_COLS=%d, editor_active=%d", BOARD_ROWS, BOARD_COLS, editor_active);
        logged = true;
    }
    
    // Afficher un rectangle de test au centre
    DrawRectangleRec((Rectangle){600, 300, 200, 100}, RED);
    DrawText("TEST", 650, 330, 20, WHITE);
    
    // Afficher toutes les tuiles
    for (int y = 0; y < BOARD_ROWS; y++)
    {
        for (int x = 0; x < BOARD_COLS; x++)
        {
            const Tile *t = &board->tiles[y][x];

            // Fond de la tuile
            DrawRectangle(
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                LIGHTGRAY);

            // Textures des tuiles
            for (int i = 0; i < t->layerCount; i++)
            {
                int idx = t->layers[i];
                if (idx >= 0 && idx < gTileTextureCount)
                {
                    // Vérifier que la texture est chargée (non vide)
                    if (gTileTextures[idx].id > 0)
                    {
                        DrawTexture(
                            gTileTextures[idx],
                            x * TILE_SIZE,
                            y * TILE_SIZE,
                            WHITE);
                    }
                }
            }

            // Contour de tuile (debug)
            if (editor_active) {
                DrawRectangleLines(
                    x * TILE_SIZE,
                    y * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE,
                    DARKGRAY);
            }
        }
    }

    // Afficher les ennemis au-dessus des tuiles
    DrawEnemies(board);

    // Afficher le joueur par-dessus les tuiles
    if (board->player.texture_id >= 0 && board->player.texture_id < gTileTextureCount)
    {
        // Déterminer quelle texture afficher en fonction de l'état du combat
        int texture_to_draw = board->player.texture_id;
        
        extern CombatState gCombatState;
        if (gCombatState.combat_overlay_active && gCombatState.knight.state == KNIGHT_ATTACKING) {
            // Utiliser les sprites d'attaque (103-106) selon le frame
            texture_to_draw = 103 + gCombatState.knight.attack_animation_frame;
        } else if (gCombatState.combat_overlay_active && gCombatState.knight.state == KNIGHT_DEFENDING) {
            // Utiliser les sprites de défense (107-108)
            texture_to_draw = 107;
        }
        
        Texture2D player_texture = gTileTextures[texture_to_draw];
        // Vérifier que la texture est chargée
        if (player_texture.id > 0)
        {
            // Utiliser DrawTexturePro pour pouvoir faire un mirroir selon la direction
            float sourceWidth = (float)(board->player.lastDirection > 0 ? player_texture.width : -player_texture.width);
            Rectangle source = {0, 0, sourceWidth, (float)player_texture.height};
            Rectangle dest = {(int)board->player.pixelX, (int)board->player.pixelY, TILE_SIZE, TILE_SIZE};
            DrawTexturePro(player_texture, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
        }
        else
        {
            // Fallback : afficher un carré rouge si pas de texture
            DrawRectangle(
                (int)board->player.pixelX,
                (int)board->player.pixelY,
                TILE_SIZE,
                TILE_SIZE,
                RED);
        }
        
    }

    // Affichage du nom de carte en haut à gauche
    if (editor_active) {
        char displayText[128];
        if (isNamingMap) {
            snprintf(displayText, sizeof(displayText), "Map name: %s_", currentMapName);
            DrawText(displayText, 10, 10, 20, YELLOW);
            DrawText("(Press Enter to confirm, Tab to cancel)", 10, 35, 14, GRAY);
        } else {
            snprintf(displayText, sizeof(displayText), "Map: %s (Tab to rename)", currentMapName);
            DrawText(displayText, 10, 10, 20, WHITE);
            DrawText("Ctrl+S to save | Ctrl+L to load", 10, 35, 14, GRAY);
        }
    }

    // Afficher l'overlay du combat par-dessus la map
    extern CombatState gCombatState;
    
    // Afficher game over si le joueur est mort
    if (game_over) {
        Vector2 mousePos = GetMousePosition();
        
        // Fond semi-transparent noir
        Color darkOverlay = {0, 0, 0, 200};
        DrawRectangle(0, 0, 1088, 704, darkOverlay);
        
        // Texte GAME OVER
        const char *gameOverText = "GAME OVER";
        
        int gameOverWidth = MeasureText(gameOverText, 80);
        DrawText(gameOverText, (PLAYABLE_ZONE_WIDTH - gameOverWidth) / 2, (PLAYABLE_ZONE_HEIGTH / 2) - 120, 80, RED);
        
        // Définir les rectangles des boutons
        Rectangle btnReessaie = {((PLAYABLE_ZONE_WIDTH / 2) - 70) - 100, 400, 140, 60};
        Rectangle btnAbandoner = {((PLAYABLE_ZONE_WIDTH / 2) - 70) + 100, 400, 140, 60};
        
        // Couleurs des boutons (changent si survol)
        Color colorReessaie = CheckCollisionPointRec(mousePos, btnReessaie) ? (Color){0, 200, 0, 255} : (Color){0, 150, 0, 255};
        Color colorAbandoner = CheckCollisionPointRec(mousePos, btnAbandoner) ? (Color){200, 0, 0, 255} : (Color){150, 0, 0, 255};
        
        // Dessiner les boutons
        DrawRectangleRec(btnReessaie, colorReessaie);
        DrawRectangleLinesEx(btnReessaie, 2, WHITE);
        DrawText("REESSAYER", (int)btnReessaie.x + 15, (int)btnReessaie.y + 18, 18, WHITE);
        
        DrawRectangleRec(btnAbandoner, colorAbandoner);
        DrawRectangleLinesEx(btnAbandoner, 2, WHITE);
        DrawText("ABANDONNER", (int)btnAbandoner.x + 10, (int)btnAbandoner.y + 18, 18, WHITE);
    }
    
    DrawCombat(&gCombatState);
}