#include "raylib.h"
#include "game.h"
#include "battle.h"
#include "marchand.h"
#include <stdio.h>
#include <string.h>

#define SHOP_TILE_ID 120

int player_money = 0;
bool is_in_shop = false;

extern double hitbox_height;
extern double hitbox_width;
extern double attack_power;

// Modificateurs de stats
double force_modifier = 1.0;
double defense_modifier = 1.0;
double speed_modifier = 1.0;
double range_modifier = 1.0;
double attack_speed_modifier = 1.0;
double rage_modifier = 1.0;
int avarice_modifier = 1;

ShopItem shop_items[MAX_SHOP_ITEMS];  // Tableau contenant tous les items du shop

// Initialise les items du shop (à appeler au démarrage du jeu)
void InitShopItems(void) {
    // Position de départ pour les icônes
    int shop_posX = 368;
    int shop_posY = 20;
    int icon_size = 64;      // Taille d'une icône
    int padding = 26;        // Espace entre les icônes
    int items_per_row = 4;   // Nombre d'icônes par ligne
    
    // Initialisation de chaque item
    for (int i = 0; i < MAX_SHOP_ITEMS; i++) {
        int row = i / items_per_row;
        int col = i % items_per_row;
        
        shop_items[i].type = ITEM_NONE;
        shop_items[i].bounds = (Rectangle){
            shop_posX + padding + col * (icon_size + padding),
            shop_posY + padding + row * (icon_size + padding),
            icon_size,
            icon_size
        };
        shop_items[i].price = 0;
        shop_items[i].isActive = false;
        shop_items[i].tintColor = WHITE;
        shop_items[i].currentStack = 0;

    }
    
    // Exemple de configuration d'items (tu peux adapter selon tes besoins)
    shop_items[0].type = FORCE_UPGRADE;
    shop_items[0].price = 30;
    shop_items[0].isActive = true;
    shop_items[0].icon = LoadTexture("assets/hud/force.png");
    shop_items[0].maxStack = 5;
    shop_items[0].itemDesc = "FORCE : fait plus de dégâts aux ennemis (max 5)";
    shop_items[0].itemName = "Force";
    
    shop_items[1].type = DEFENSE_UPGRADE;
    shop_items[1].price = 30;
    shop_items[1].isActive = true;
    shop_items[1].icon = LoadTexture("assets/hud/defense.png");
    shop_items[1].maxStack = 5;
    shop_items[1].itemDesc = "DEFENSE : les ennemis infligent moins de dégâts (max 5)";
    shop_items[1].itemName = "Défense";
    
    shop_items[2].type = SPEED_UPGRADE;
    shop_items[2].price = 30;
    shop_items[2].isActive = true;
    shop_items[2].icon = LoadTexture("assets/hud/vitesse.png");
    shop_items[2].maxStack = 5;
    shop_items[2].itemDesc = "VITESSE : déplacements plus rapide (max 5)";
    shop_items[2].itemName = "Vitesse";

    shop_items[3].type = RANGE_UPGRADE;
    shop_items[3].price = 30;
    shop_items[3].isActive = true;
    shop_items[3].icon = LoadTexture("assets/hud/portee.png");
    shop_items[3].maxStack = 5;
    shop_items[3].itemDesc = "PORTEE : touche les ennemis de plus loin (max 5)";
    shop_items[3].itemName = "Portée";

    shop_items[4].type = ATTACK_SPEED_UPGRADE;
    shop_items[4].price = 30;
    shop_items[4].isActive = true;
    shop_items[4].icon = LoadTexture("assets/hud/vitesse d'attaque.png");
    shop_items[4].maxStack = 5;
    shop_items[4].itemDesc = "VITESSE D'ATTAQUE : tape plus vite (max 5)";
    shop_items[4].itemName = "ATK speed";

    shop_items[5].type = HEALTH_UPGRADE;
    shop_items[5].price = 25;
    shop_items[5].isActive = true;
    shop_items[5].icon = LoadTexture("assets/hud/PV.png");
    shop_items[5].maxStack = 10;
    shop_items[5].itemDesc = "POTION DE VIE : remet la barre de vie au max (max 10)";
    shop_items[5].itemName = "Potion";

    shop_items[6].type = AVARICE_CURSE;
    shop_items[6].price = 150;
    shop_items[6].isActive = true;
    shop_items[6].icon = LoadTexture("assets/hud/avarice.png");
    shop_items[6].maxStack = 1;
    shop_items[6].itemDesc = "AVARICE : plus d'argent gagné mais beaucoup moins de défense (max 1)";
    shop_items[6].itemName = "Avarice";

    shop_items[7].type = RAGE_CURSE;
    shop_items[7].price = 150;
    shop_items[7].isActive = true;
    shop_items[7].icon = LoadTexture("assets/hud/rage.png");
    shop_items[7].maxStack = 1;
    shop_items[7].itemDesc = "RAGE : moins de défense et de vie pour plus de dégâts (max 1)";
    shop_items[7].itemName = "Rage";
}

// Décharge les textures du shop
void UnloadShopItems(void) {
    for (int i = 0; i < MAX_SHOP_ITEMS; i++) {
        if (shop_items[i].isActive) {
            UnloadTexture(shop_items[i].icon);
        }
    }
}

// Vérifie si un item a été cliqué et exécute son effet
void HandleShopItemClick(Vector2 mousePos, CombatState *combatState) {
    if (!is_in_shop) return;
    
    // Parcours tous les items du shop
    for (int i = 0; i < MAX_SHOP_ITEMS; i++) {
        // Vérifie si l'item est actif et si la souris est dessus
        if ((shop_items[i].isActive && CheckCollisionPointRec(mousePos, shop_items[i].bounds)) && (shop_items[i].currentStack < shop_items[i].maxStack)) {
            
            // Vérifie si le joueur a assez d'argent
            if (player_money >= shop_items[i].price) {
                TraceLog(LOG_INFO, "Item acheté: %d", shop_items[i].type);
                
                // Exécute l'effet selon le type d'item
                switch (shop_items[i].type) {
                    case FORCE_UPGRADE:
                        TraceLog(LOG_INFO, "Force +");
                        force_modifier += 0.5;
                        attack_power *= force_modifier;
                        shop_items[i].currentStack ++;
                        break;
                        
                    case DEFENSE_UPGRADE:
                        TraceLog(LOG_INFO, "Défense +");
                        defense_modifier -= 0.05;
                        shop_items[i].currentStack ++;
                        break;
                        
                    case SPEED_UPGRADE:
                        TraceLog(LOG_INFO, "Vitesse +");
                        speed_modifier += 0.1;
                        shop_items[i].currentStack ++;
                        break;
                        
                    case RANGE_UPGRADE:
                        TraceLog(LOG_INFO, "Portée +");
                        range_modifier += 0.15;
                        shop_items[i].currentStack ++;
                        break;
                        
                    case ATTACK_SPEED_UPGRADE:
                        TraceLog(LOG_INFO, "Vitesse d'attaque +");
                        attack_speed_modifier -= 0.15;
                        shop_items[i].currentStack ++;
                        break;
                        
                    case HEALTH_UPGRADE:
                        TraceLog(LOG_INFO, "PV +");
                        gCombatState.knight.hp = gCombatState.knight.max_hp;
                        shop_items[i].currentStack ++;
                        break;
                        
                    case AVARICE_CURSE:
                        TraceLog(LOG_INFO, "Avarice activée");
                        shop_items[i].currentStack ++;

                        // Effets négatifs
                        defense_modifier += 2.0;

                        // Effets positifs
                        avarice_modifier = 4.0;
                        break;
                        
                    case RAGE_CURSE:
                        TraceLog(LOG_INFO, "Rage activée");
                        shop_items[i].currentStack ++;

                        // Effets négatifs
                        gCombatState.knight.max_hp /= 3;
                        gCombatState.knight.hp /= 3;
                        defense_modifier += 0.2;

                        // EFfets positifs
                        force_modifier += 0.15;
                        attack_speed_modifier -= 0.2;
                        speed_modifier += 0.2;
                        break;
                        
                    default:
                        break;
                }
                
                // Déduit l'argent du joueur
                player_money -= shop_items[i].price;
                
            } else {
                TraceLog(LOG_WARNING, "Pas assez d'argent pour acheter cet item!");
            }
            
            return;  // Sort de la fonction après avoir traité un clic
        }
    }
}

// Gère le survol de la souris sur les items (pour effet visuel)
void UpdateShopItemsHover(Vector2 mousePos) {
    if (!is_in_shop) return;
    
    for (int i = 0; i < MAX_SHOP_ITEMS; i++) {
        if (shop_items[i].isActive) {
            // Change la couleur si la souris survole l'item
            if (CheckCollisionPointRec(mousePos, shop_items[i].bounds)) {

                // Infobulles
                Vector2 m = GetMousePosition();
                int textWidth = MeasureText(shop_items[i].itemDesc, 21);
                DrawRectangle((m.x + 2), (m.y + 2), (textWidth + 6), 25, GRAY);
                DrawText(shop_items[i].itemDesc, (m.x + 4), (m.y + 4), 21, WHITE);

                // Overlaw de couleur
                if ((player_money >= shop_items[i].price) && (shop_items[i].currentStack < shop_items[i].maxStack)) {
                    shop_items[i].tintColor = GREEN;
                } else {
                    shop_items[i].tintColor = RED;
                }
            } else {
                shop_items[i].tintColor = WHITE;   // Couleur normale
            }
        }
    }
}

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

// Dessine le shop et tous ses items
int DrawShop(bool is_in_shop) {
    if (is_in_shop) {
        int shop_posX = 368;
        int shop_posY = 20;
        Color shop_background_color = {0x80, 0x53, 0x00, 0xff};
        
        // Dessine le fond du shop
        DrawRectangle(shop_posX, shop_posY, 384, 206, shop_background_color);
        
        // Dessine chaque item du shop
        for (int i = 0; i < MAX_SHOP_ITEMS; i++) {
            if (shop_items[i].isActive) {
                // Dessine le fond de l'icône
                DrawRectangleRec(shop_items[i].bounds, DARKGRAY);
                
                DrawTexturePro(
                    shop_items[i].icon,
                    (Rectangle){0, 0, shop_items[i].icon.width, shop_items[i].icon.height},
                    shop_items[i].bounds,
                    (Vector2){0, 0},
                    0.0f,
                    shop_items[i].tintColor
                );
                
                // Dessine le prix en dessous de l'icône
                char price_text[16];
                sprintf(price_text, "%d$", shop_items[i].price);
                DrawText(
                    price_text,
                    shop_items[i].bounds.x + 5,
                    shop_items[i].bounds.y + shop_items[i].bounds.height - 15,
                    15,
                    GOLD
                );
            }
        }
    }
    return 0;
}
