#include "game.h"

extern Texture2D gTileTextures[];
extern int gTileTextureCount;


// ******************************************
//

static void TileClear(Tile *t)
{
    t->layerCount = 0;
    for (int i = 0; i < MAX_LAYERS; i++)
    {
        t->layers[i] = -1;
    }
}

static bool TilePush(Tile *t, int texIndex)
{
    if (t->layerCount >= MAX_LAYERS)
        return false;
    t->layers[t->layerCount++] = texIndex;
    return true;
}

static int TilePop(Tile *t)
{
    if (t->layerCount <= 0)
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

            // couche 0 : sol
            int groundIndex = (x+y) % 2; // ex: desert ou mer
            TilePush(t, groundIndex);

            // disposition des marteau sur la diagonale
            if (x == y)
            {
                int objectIndex = 2; // ex: pierre, arbre…
                TilePush(t, objectIndex);
            }
        }
    }
}

void GameUpdate(Board *board, float dt)
{
    Vector2 m = GetMousePosition();
    
    // Gestion des entrées souris sur les tuiles
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        int tileX = (int)(m.x) / TILE_SIZE;
        int tileY = (int)(m.y) / TILE_SIZE;
        
        TraceLog(LOG_INFO,
            "MOUSE DOWN at x=%.1f y=%.1f corresponding tile (%d, %d)",
            m.x, m.y, tileX, tileY);
            
            Tile *t = &board->tiles[tileY][tileX];
            if (t->layerCount > 1)
            {
                TilePop(t);
            }
            else
            {
                int objectIndex = 2;
                TilePush(t, objectIndex);
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
