#include "raylib.h"
#include "game.h"

bool editor_active = false;

int map_editor(Board *board) {

    if (editor_active == false) {
        //Affichage de l'éditeur de carte
        for (int y = 0; y < BOARD_ROWS; y++)
        {
            for (int x = 0; x < BOARD_COLS; x++)
            {
                Tile *t = &board->tiles[y][x];

                int groundIndex = 0;

                if ((x % 43) > 33) {
                    TileClear(t);
                    groundIndex = 2; //Tuile violette
                    TilePush(t, groundIndex);
                }
            }
        }
        editor_active = true;

    } else {
        //Retour à l'inventaire
        for (int y = 0; y < BOARD_ROWS; y++)
        {
            for (int x = 0; x < BOARD_COLS; x++)
            {
                Tile *t = &board->tiles[y][x];

                int groundIndex = 0;

                if (x % 43 > 33) {
                    TileClear(t);
                    groundIndex = 1; //Tuile marron
                    TilePush(t, groundIndex);
                }
            }
        }
        editor_active = false;
    }

    return 0;
}