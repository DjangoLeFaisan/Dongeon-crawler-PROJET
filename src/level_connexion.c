#include "raylib.h"
#include "game.h"
#include "map_io.h"
#include "marcher.h"
#include "enemy.h"
#include <stdio.h>
#include <string.h>

// Fonction pour nettoyer les ennemis d'une board
static void ClearEnemies(Board *board) {
    for (int i = 0; i < board->enemy_count; i++) {
        if (board->enemies[i] != NULL) {
            DestroyEnemy(board->enemies[i]);
            board->enemies[i] = NULL;
        }
    }
    board->enemy_count = 0;
}

// Fonction pour spawner les ennemis dans un étage
static void SpawnEnemiesForStage(Board *board, int stage_number) {
    // Nettoyer les anciens ennemis
    ClearEnemies(board);
    
    // Spawn des ennemis selon l'étage
    // Vous pouvez adapter le nombre et les positions selon vos besoins
    switch(stage_number) {
        case 1:
            // Étage 1 : 2 ennemis
            if (board->enemy_count < MAX_ENEMIES) {
                board->enemies[board->enemy_count] = CreateEnemy(5, 5, 20, 5);
                if (board->enemies[board->enemy_count]) {
                    board->enemies[board->enemy_count]->texture_id = 110;  // OrcVert idle droite
                    board->enemy_count++;
                }
            }
            if (board->enemy_count < MAX_ENEMIES) {
                board->enemies[board->enemy_count] = CreateEnemy(15, 10, 20, 5);
                if (board->enemies[board->enemy_count]) {
                    board->enemies[board->enemy_count]->texture_id = 110;
                    board->enemy_count++;
                }
            }
            break;
        case 2:
            // Étage 2 : 3 ennemis
            if (board->enemy_count < MAX_ENEMIES) {
                board->enemies[board->enemy_count] = CreateEnemy(8, 8, 25, 6);
                if (board->enemies[board->enemy_count]) {
                    board->enemies[board->enemy_count]->texture_id = 110;
                    board->enemy_count++;
                }
            }
            if (board->enemy_count < MAX_ENEMIES) {
                board->enemies[board->enemy_count] = CreateEnemy(20, 5, 25, 6);
                if (board->enemies[board->enemy_count]) {
                    board->enemies[board->enemy_count]->texture_id = 110;
                    board->enemy_count++;
                }
            }
            if (board->enemy_count < MAX_ENEMIES) {
                board->enemies[board->enemy_count] = CreateEnemy(12, 15, 25, 6);
                if (board->enemies[board->enemy_count]) {
                    board->enemies[board->enemy_count]->texture_id = 110;
                    board->enemy_count++;
                }
            }
            break;
        default:
            // Autres étages : 2 ennemis par défaut
            if (board->enemy_count < MAX_ENEMIES) {
                board->enemies[board->enemy_count] = CreateEnemy(7, 7, 20, 5);
                if (board->enemies[board->enemy_count]) {
                    board->enemies[board->enemy_count]->texture_id = 110;
                    board->enemy_count++;
                }
            }
            if (board->enemy_count < MAX_ENEMIES) {
                board->enemies[board->enemy_count] = CreateEnemy(18, 12, 20, 5);
                if (board->enemies[board->enemy_count]) {
                    board->enemies[board->enemy_count]->texture_id = 110;
                    board->enemy_count++;
                }
            }
            break;
    }
    
    TraceLog(LOG_INFO, "Spawned %d enemies for stage %d", board->enemy_count, stage_number);
}



int current_level = 1;
bool special_level = false;
char next_level[256] = "";

// Charger le niveau suivant
bool LoadNextLevel(Board *board) {
    if (special_level) {
        
        // Construire le chemin du fichier : "maps/Etage{level}.map"
        snprintf(next_level, sizeof(next_level), "maps/Etage%d.map", current_level);
        
        if (MapLoad(board, next_level)) {
            TraceLog(LOG_INFO, "Carte chargée avec succès: %s", next_level);
            
            // Spawner les ennemis pour cet étage
            SpawnEnemiesForStage(board, current_level);
            
            current_level++;
            special_level = false;
            return true;
        } else {
            TraceLog(LOG_ERROR, "Erreur lors du chargement de la carte: %s", next_level);
            return false;
        }
    } else {

        if (MapLoad(board, "maps/couloir_defaul.map")) {
            TraceLog(LOG_INFO, "Carte chargée avec succès: maps/couloir_defaul.map");
            
            // Pas d'ennemis dans les couloirs
            ClearEnemies(board);
            
            special_level = true;
            return true;
        } else {
            TraceLog(LOG_ERROR, "Erreur lors du chargement de la carte: maps/couloir_defaul.map");
            return false;
        }
    }
    return false;
}
