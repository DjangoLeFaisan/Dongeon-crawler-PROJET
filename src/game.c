#include "raylib.h"
#include "game.h"
#include "map_io.h"


extern Texture2D gTileTextures[];
extern int gTileTextureCount;
extern bool editor_active;
int objectIndex = 4;
static int lastPreviewX = -1;
static int lastPreviewY = -1;

#define MAP_FILENAME "maps/map_default.map"


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

// ******************************************
// Gestion du board et des entrées

void GameInit(Board *board)
{
    for (int y = 0; y < BOARD_ROWS; y++)
    {
        for (int x = 0; x < BOARD_COLS; x++)
        {
            Tile *t = &board->tiles[y][x];
            TileClear(t);

            int groundIndex = 0;

            // couche 0 : sol
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
                "Tuile placée au coordonnées x=%.1f y=%.1f à la tuile correspondante : (%d, %d)",
                m.x, m.y, tileX, tileY);
                
                Tile *t = &board->tiles[tileY][tileX];
                TilePush(t, objectIndex);
        
        } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && (tileX <= 33)) // Enlever la tuile la plus haute
        {
            TraceLog(LOG_INFO,
                "Tuile enlevée au coordonnées x=%.1f y=%.1f à la tuile correspondante : (%d, %d)",
                m.x, m.y, tileX, tileY);
                
                Tile *t = &board->tiles[tileY][tileX];
                TilePop(t);

        } else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) // Sélectionner la tuile la plus haute
        {
            TraceLog(LOG_INFO,
                "Type de tuile sélectionnée au coordonnées x=%.1f y=%.1f à la tuile correspondante : (%d, %d)",
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
        
        // Sauvegarde (Ctrl+S)
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
            if (MapSave(board, MAP_FILENAME)) {
                TraceLog(LOG_INFO, "Map saved to %s", MAP_FILENAME);
            } else {
                TraceLog(LOG_ERROR, "Failed to save map");
            }
        }

        // Chargement (Ctrl+L)
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_L)) {
            if (MapLoad(board, MAP_FILENAME)) {
                TraceLog(LOG_INFO, "Map loaded from %s", MAP_FILENAME);
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
    for (int y = 0; y < BOARD_ROWS; y++)
    {
        for (int x = 0; x < BOARD_COLS; x++)
        {
            const Tile *t = &board->tiles[y][x];

            // fond “vide” au cas où
            DrawRectangle(
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                LIGHTGRAY);

            // dessine chaque couche dans l'ordre
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

            // contour de la tuile (debug)
            DrawRectangleLines(
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                DARKGRAY);
        }
    }
}
