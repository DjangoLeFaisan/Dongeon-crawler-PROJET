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
    
    // Gestion de la musique : Field dans les couloirs, Combat dans Etage1-8, Boss Final dans Etage9
    extern Music gBackgroundMusic;
    extern bool gMusicPlaying;
    extern Music gCombatMusic;
    extern bool gCombatMusicPlaying;
    extern Music gBossFinalMusic;
    extern bool gBossFinalMusicPlaying;
    
    // Vérifier si c'est Etage9 (boss final)
    bool isBossFinalStage = (strstr(filename, "Etage9") != NULL);
    
    // Vérifier si c'est un étage de combat (Etage1 à Etage8)
    bool isCombatStage = (!isBossFinalStage && (strstr(filename, "Etage1") != NULL ||
                          strstr(filename, "Etage2") != NULL ||
                          strstr(filename, "Etage3") != NULL ||
                          strstr(filename, "Etage4") != NULL ||
                          strstr(filename, "Etage5") != NULL ||
                          strstr(filename, "Etage6") != NULL ||
                          strstr(filename, "Etage7") != NULL ||
                          strstr(filename, "Etage8") != NULL));
    
    if (isBossFinalStage) {
        // Étage du boss final : jouer la musique du boss final
        // Arrêter les autres musiques
        if (gBackgroundMusic.frameCount > 0 && IsMusicStreamPlaying(gBackgroundMusic)) {
            PauseMusicStream(gBackgroundMusic);
            gMusicPlaying = false;
        }
        if (gCombatMusic.frameCount > 0 && IsMusicStreamPlaying(gCombatMusic)) {
            PauseMusicStream(gCombatMusic);
            gCombatMusicPlaying = false;
        }
        // Jouer la musique du boss final
        if (gBossFinalMusic.frameCount > 0 && !IsMusicStreamPlaying(gBossFinalMusic)) {
            PlayMusicStream(gBossFinalMusic);
            gBossFinalMusicPlaying = true;
            TraceLog(LOG_INFO, "Musique du boss final activée");
        }
    } else if (isCombatStage) {
        // Étage de combat : jouer la musique de combat
        // Arrêter les autres musiques
        if (gBackgroundMusic.frameCount > 0 && IsMusicStreamPlaying(gBackgroundMusic)) {
            PauseMusicStream(gBackgroundMusic);
            gMusicPlaying = false;
        }
        if (gBossFinalMusic.frameCount > 0 && IsMusicStreamPlaying(gBossFinalMusic)) {
            PauseMusicStream(gBossFinalMusic);
            gBossFinalMusicPlaying = false;
        }
        // Jouer la musique de combat si elle ne joue pas
        if (gCombatMusic.frameCount > 0 && !IsMusicStreamPlaying(gCombatMusic)) {
            PlayMusicStream(gCombatMusic);
            gCombatMusicPlaying = true;
            TraceLog(LOG_INFO, "Musique de combat activée");
        }
    } else {
        // Couloir ou autre : jouer la musique de fond
        // Arrêter les autres musiques
        if (gCombatMusic.frameCount > 0 && IsMusicStreamPlaying(gCombatMusic)) {
            PauseMusicStream(gCombatMusic);
            gCombatMusicPlaying = false;
        }
        if (gBossFinalMusic.frameCount > 0 && IsMusicStreamPlaying(gBossFinalMusic)) {
            PauseMusicStream(gBossFinalMusic);
            gBossFinalMusicPlaying = false;
        }
        // Jouer la musique de fond si elle ne joue pas
        if (gBackgroundMusic.frameCount > 0 && !IsMusicStreamPlaying(gBackgroundMusic)) {
            PlayMusicStream(gBackgroundMusic);
            gMusicPlaying = true;
            TraceLog(LOG_INFO, "Musique de fond activée");
        }
    }
    
    // Spawn le boss si c'est Etage8
    extern void SpawnBoss(Board *board, const char *mapName);
    SpawnBoss(board, filename);
    
    // S'assurer que l'overlay de combat est activé pour les étages
    bool isCombatOrBossStage = (strstr(filename, "Etage") != NULL);
    if (isCombatOrBossStage) {
        extern CombatState gCombatState;
        gCombatState.combat_overlay_active = true;
        TraceLog(LOG_INFO, "Combat overlay forcé à ACTIF pour %s", filename);
    }
    
    TraceLog(LOG_INFO, "Map loaded successfully: %s", filename);
    return true;
}