#include "map_io.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

#define PLAYABLE_COLS 34  // colonnes 0-33
#define SPAWN_DOOR_INDEX 27 // Définit quelle tuile fait apparaître le joueur

bool MapSave(const Board *board, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        TraceLog(LOG_ERROR, "Failed to open file for writing: %s", filename);
        return false;
    }

    // Ecrire un entête simple (version, dimensions)
    unsigned int version = 1;
    unsigned int rows = BOARD_ROWS;
    unsigned int cols = PLAYABLE_COLS;

    fwrite(&version, sizeof(unsigned int), 1, file);
    fwrite(&rows, sizeof(unsigned int), 1, file);
    fwrite(&cols, sizeof(unsigned int), 1, file);

    // Ecrire les tuiles (uniquement colonnes 0-33)
    Vector2 m = GetMousePosition();
    int tileX = (int)(m.x) / TILE_SIZE;
    int tileY = (int)(m.y) / TILE_SIZE;
    Tile *t = &board->tiles[tileY][tileX];
    TilePop(t);

    for (int y = 0; y < BOARD_ROWS; y++) {
        for (int x = 0; x < PLAYABLE_COLS; x++) {
            const Tile *t = &board->tiles[y][x];
            fwrite(&t->layerCount, sizeof(int), 1, file);
            fwrite(t->layers, sizeof(int), MAX_LAYERS, file);
        }
    }

    fclose(file);
    TraceLog(LOG_INFO, "Map saved successfully: %s", filename);
    return true;
}

bool MapLoad(Board *board, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        TraceLog(LOG_ERROR, "Failed to open file for reading: %s", filename);
        return false;
    }

    // Lire et vérifier l'entête
    unsigned int version, rows, cols;
    fread(&version, sizeof(unsigned int), 1, file);
    fread(&rows, sizeof(unsigned int), 1, file);
    fread(&cols, sizeof(unsigned int), 1, file);

    if (version != 1 || rows != BOARD_ROWS || cols != PLAYABLE_COLS) {
        TraceLog(LOG_ERROR, "Map file format incompatible or corrupted");
        fclose(file);
        return false;
    }

    // Lire les tuiles (colonnes 0-33)
    for (int y = 0; y < BOARD_ROWS; y++) {
        for (int x = 0; x < PLAYABLE_COLS; x++) {
            Tile *t = &board->tiles[y][x];
            fread(&t->layerCount, sizeof(int), 1, file);
            fread(t->layers, sizeof(int), MAX_LAYERS, file);

            // Fait apparaître le joueur si la tuile d'apparition est détectée
            int currentTextureIndex = t->layers[t->layerCount - 1];
            if (currentTextureIndex == SPAWN_DOOR_INDEX) {
                board->player.gridX = x;
                board->player.gridY = (y + 1);
                board->player.pixelX = x * TILE_SIZE;
                board->player.pixelY = (y + 1) * TILE_SIZE;
                TraceLog(LOG_INFO, "Player spawn position set at (%d, %d)", x, (y + 1));
            }
        }
    }

    fclose(file);
    TraceLog(LOG_INFO, "Map loaded successfully: %s", filename);
    return true;
}