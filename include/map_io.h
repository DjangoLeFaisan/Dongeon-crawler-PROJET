#ifndef MAP_IO_H
#define MAP_IO_H

#include "game.h"
#include <stdbool.h>

// Sauvegarde une carte (colonnes 0-33)
bool MapSave(const Board *board, const char *filename);

// Charge une carte
bool MapLoad(Board *board, const char *filename);

#endif
