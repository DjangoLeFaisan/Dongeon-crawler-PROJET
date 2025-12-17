#include "game.h"
#include "map_editor.h"
#include "marchand.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define PLAY_AREA_WIDTH 1088

extern ShopItem shop_items[MAX_SHOP_ITEMS];
extern Texture2D money_texture;
extern Texture2D gTileTextures[];
extern int ennemies_killed;
extern int ennemies_to_kill;

char player_money_text[9];
char ennemies_left_text[32];
char stats_display[64] = "";

int DrawInventory(bool editor_active, int player_money) {
    if (!editor_active) {
        // Monnaie
        sprintf(player_money_text, "%d", player_money);
        DrawTextureEx(money_texture, (Vector2){(PLAY_AREA_WIDTH + 16), 16}, 0.0f, 1.5f, WHITE);
        DrawText(player_money_text, (PLAY_AREA_WIDTH + 80), 16, 48, GOLD);

        // Ennemis restants
        sprintf(ennemies_left_text, "Ennemis à tuer : %d/%d", ennemies_killed, ennemies_to_kill);
        DrawTextureEx(gTileTextures[111], (Vector2){(PLAY_AREA_WIDTH + 16), 80}, 0.0f, 1.5f, WHITE);

        if (ennemies_killed >= ennemies_to_kill) {
            DrawText(ennemies_left_text, (PLAY_AREA_WIDTH + 80), 96, 16, GREEN);
        } else {
            DrawText(ennemies_left_text, (PLAY_AREA_WIDTH + 80), 96, 16, WHITE);
        }
        // Barre de séparation
        DrawLineEx((Vector2){(PLAY_AREA_WIDTH + 16), 140}, (Vector2){(PLAY_AREA_WIDTH + 272), 140}, 5.0, BROWN);    

        // Statistiques
        DrawRectangle((PLAY_AREA_WIDTH + 16), 150, 256, 548, DARKBROWN);   
        for (int i = 0; i < MAX_SHOP_ITEMS; i++) {
            DrawTextureEx(shop_items[i].icon, (Vector2){(PLAY_AREA_WIDTH + 24), ((i * (48 + 20))) + 160}, 0.0f, 1.5f, WHITE);
            snprintf(stats_display, sizeof(stats_display), "%s : %d/%d", shop_items[i].itemName, shop_items[i].currentStack, shop_items[i].maxStack);
            DrawText(stats_display, (PLAY_AREA_WIDTH + 85), (i * (48 + 20)) + 180, 20, WHITE);
        }
    }
    return 0;
}