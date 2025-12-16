#include "raylib.h"
#include "game.h"
#include <stdio.h>

#define SHOP_TILE_ID 120

bool is_in_shop = false;

// Alterne entre le shop ouvert et fermé
int ToggleShopInventory(int tileIndex) {
    if ((tileIndex == SHOP_TILE_ID) && (is_in_shop == false)) {
        is_in_shop = true;
        TraceLog(LOG_INFO, "Le joueur a ouvert le shop");
    } else if ((tileIndex != SHOP_TILE_ID) && (is_in_shop == true)) {
        is_in_shop = false;
        TraceLog(LOG_INFO, "Le joueur a fermé le shop");
    }
    return 0;
}

int DrawShop(bool is_in_shop) {
    if (is_in_shop) {
        int shop_posX = 368;
        int shop_posY = 20;
        Color shop_background_color = {0x80, 0x53, 0x00, 0xff};
        DrawRectangle(shop_posX, shop_posY, 384, 220, shop_background_color);  
    }
    return 0;
}
