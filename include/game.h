#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "player.h"
#include "enemy.h"

extern Sound gEnemyMusic;
extern Sound gVictoryMusic;
extern Sound gDeathSound;
extern Music gBackgroundMusic;

#define TILE_SIZE 32
#define BOARD_COLS 43
#define BOARD_ROWS 24
#define MAX_LAYERS 8

typedef struct
{
    int layers[MAX_LAYERS];     // indices dans gTileTextures
    int layerCount;             // nombre de couches utilisées
} Tile;

typedef struct Board
{
    Player player;
    Enemy enemies[16];
    int enemyCount;
    Tile tiles[BOARD_ROWS][BOARD_COLS];
} Board;

int map_editor(Board *board);

void TileClear(Tile *t);
bool TilePush(Tile *t, int texIndex);
int TilePop(Tile *t);

void GameInit(Board *board);
void GameUpdate(Board *board, float dt);
void GameDraw(const Board *board);

// Fonctions Boss
void SpawnBoss(Board *board, const char *mapName);
void UpdateBoss(Board *board, float dt);
void DrawBoss(const Board *board);
void ResetBoss(void);
bool CheckBossCollision(int gridX, int gridY);
void DamageBoss(int damage);
void BossAttackPlayer(Board *board, int damage);
Boss* GetBoss(void);
bool CheckBossHitThisSwing(void);
void SetBossHitThisSwing(bool value);

#endif

