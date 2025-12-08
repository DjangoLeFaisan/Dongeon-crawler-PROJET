#include "raylib.h"
#include "game.h"

bool editor_active = false;
extern int gTileTextureCount;

int map_editor(Board *board) {

    if (editor_active == false) {
        //Affichage de l'éditeur de carte
        int groundIndex = 0;
        int tile_list_index = 4;
        for (int y = 0; y < BOARD_ROWS; y++)
        {
            for (int x = 0; x < BOARD_COLS; x++)
            {
                Tile *t = &board->tiles[y][x];

                if (x > 33) {
                    TileClear(t);
                    groundIndex = 2; //Tuile violette
                    TilePush(t, groundIndex);
                }
                if (((y > 0) && (y < 21)) && ((x > 34) && (x < 42))) {
                    if (tile_list_index <= gTileTextureCount) {
                        TilePush(t, tile_list_index);
                        tile_list_index += 1;
                    }
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

                if (x > 33) {
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