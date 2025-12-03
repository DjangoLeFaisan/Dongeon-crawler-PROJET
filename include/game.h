#ifndef GAME_H
#define GAME_H

#include "raylib.h"

#define TILE_SIZE 32
#define BOARD_COLS 10
#define BOARD_ROWS 8
#define MAX_LAYERS 4

typedef struct
{
    int layers[MAX_LAYERS];     // indices dans gTileTextures
    int layerCount;             // nombre de couches utilisées
} Tile;

typedef struct
{
    Tile tiles[BOARD_ROWS][BOARD_COLS];
} Board;

void GameInit(Board *board);
void GameUpdate(Board *board, float dt);
void GameDraw(const Board *board);

#endif
