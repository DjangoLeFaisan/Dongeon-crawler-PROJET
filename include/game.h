#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "player.h"
#include "enemy.h"

#define TILE_SIZE 32
#define BOARD_COLS 43
#define BOARD_ROWS 24
#define MAX_LAYERS 8
#define MAX_ENEMIES 20

typedef struct
{
    int layers[MAX_LAYERS];     // indices dans gTileTextures
    int layerCount;             // nombre de couches utilisées
} Tile;

typedef struct
{
    Player player;
    Tile tiles[BOARD_ROWS][BOARD_COLS];
    Enemy* enemies[MAX_ENEMIES];  // Tableau d'ennemis
    int enemy_count;               // Nombre d'ennemis actuels
} Board;

int map_editor(Board *board);

void TileClear(Tile *t);
bool TilePush(Tile *t, int texIndex);
int TilePop(Tile *t);

void GameInit(Board *board);
void GameUpdate(Board *board, float dt);
void GameDraw(const Board *board);


#endif
