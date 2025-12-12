#include "raylib.h"
#include "game.h"
#include "map_io.h"
#include "marcher.h"
#include <stdio.h>
#include <string.h>



int current_level = 0;
bool special_level = false;
char next_level[256] = "";

/*

// Charger le niveau suivant
bool LoadNextLevel(Board *board) {
    if (special_level) {
        special_level = false;
        
        // Construire le chemin du fichier : "maps/Etage{level}.map"
        snprintf(next_level, sizeof(next_level), "maps/Etage%d.map", current_level);
        current_level++;
        
        if (MapLoad(board, next_level)) {
            TraceLog(LOG_INFO, "Carte chargée avec succès: %s", next_level);
            return true;
        } else {
            TraceLog(LOG_ERROR, "Erreur lors du chargement de la carte: %s", next_level);
            return false;
        }
    } else {
        special_level = true;

        if (MapLoad(board, "maps/couloir_defaul.map")) {
            TraceLog(LOG_INFO, "Carte chargée avec succès: maps/couloir_defaul.map");
            return true;
        } else {
            TraceLog(LOG_ERROR, "Erreur lors du chargement de la carte: maps/couloir_defaul.map");
            return false;
        }
    }
    return false;
}
*/