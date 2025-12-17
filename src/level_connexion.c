#include "raylib.h"
#include "game.h"
#include "map_io.h"
#include "marcher.h"
#include "battle.h"
#include "enemy.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

int current_level = 1;
bool special_level = false;
char next_level[256] = "";
int ennemies_killed = 0;
int ennemies_to_kill = 0;

// Charger le niveau suivant
bool LoadNextLevel(Board *board) {
    if (special_level) {
        
        // Construire le chemin du fichier : "maps/Etage{level}.map"
        snprintf(next_level, sizeof(next_level), "maps/Etage%d.map", current_level);
        
        if (MapLoad(board, next_level)) {
            TraceLog(LOG_INFO, "Carte chargée avec succès: %s", next_level);
            special_level = false;
            spawn_enemies_enabled = true;  // Activer le spawn pour les étages
            ennemies_killed = 0;
            ennemies_to_kill = floor((((current_level - 1) / 5.0) + 0.8) * 5);
            current_level++;
            ToggleCombatOverlay();
            SpawnEnemiesForEtage(board);
            return true;
        } else {
            TraceLog(LOG_ERROR, "Erreur lors du chargement de la carte: %s", next_level);
            return false;
        }
    } else {

        if (MapLoad(board, "maps/couloir_defaul.map")) {
            TraceLog(LOG_INFO, "Carte chargée avec succès: maps/couloir_defaul.map");
            special_level = true;
            spawn_enemies_enabled = false;  // Désactiver le spawn pour le couloir
            ennemies_killed = 0;
            ennemies_to_kill = 0;
            ToggleCombatOverlay();
            ResetEnemies(board);
            return true;
        } else {
            TraceLog(LOG_ERROR, "Erreur lors du chargement de la carte: maps/couloir_defaul.map");
            return false;
        }
    }
    return false;
}
