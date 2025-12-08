#include "raylib.h"
#include "game.h"
#include "map_io.h"
#include "marcher.h"
#include <string.h>
#include <stdio.h>


extern Texture2D gTileTextures[];
extern int gTileTextureCount;
extern bool editor_active;

int objectIndex = 4;
static int lastPreviewX = -1;
static int lastPreviewY = -1;

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
    board->player.gridX = 5;
    board->player.gridY = 5;
    board->player.speed = 1;
    board->player.texture_id = 102;
    
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
    // Gestion du joueur
    Marcher(&board->player, board); 
    
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
                        DrawTexture(
                            gTileTextures[idx],
                            x * TILE_SIZE,
                            y * TILE_SIZE,
                            WHITE);
                    }
                }

            // Contour de tuile (debug)
            DrawRectangleLines(
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                DARKGRAY);
        }
    }

    // Afficher le joueur par-dessus les tuiles
    if (board->player.texture_id >= 0 && board->player.texture_id < gTileTextureCount)
    {
        
        Texture2D player_texture = gTileTextures[board->player.texture_id];
            DrawTexture(
                player_texture,
            board->player.gridX * TILE_SIZE,
            board->player.gridY * TILE_SIZE,
                WHITE
            );
        }
        else
        {
        // Fallback : afficher un carré rouge si pas de texture
            DrawRectangle(
            board->player.gridX * TILE_SIZE,
            board->player.gridY * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                RED);
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
}
