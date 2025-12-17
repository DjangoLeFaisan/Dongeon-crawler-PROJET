#ifndef MARCHAND_H
#define MARCHAND_H

#define MAX_SHOP_ITEMS 8

// Enumération pour identifier les différents types d'items
typedef enum {
    ITEM_NONE = 0,
    FORCE_UPGRADE,
    DEFENSE_UPGRADE,
    SPEED_UPGRADE,
    RANGE_UPGRADE,
    HEALTH_UPGRADE,
    ATTACK_SPEED_UPGRADE,
    AVARICE_CURSE,
    RAGE_CURSE
} ItemType;

// Structure représentant un item dans le shop
typedef struct {
    ItemType type;           // Type d'item
    Rectangle bounds;        // Position et taille de l'icône (x, y, width, height)
    Texture2D icon;         // Texture de l'icône
    int price;              // Prix de l'item
    bool isActive;          // Si l'item est disponible à l'achat
    Color tintColor;        // Couleur pour teinter l'icône (utile pour feedback visuel)
    int maxStack; // Définit le max d'un bonus qu'on peut avoir
    int currentStack; // Variable du nombre de bonus actuel
    const char *itemDesc; // Description de l'item
    const char *itemName; // Nom de l'item
} ShopItem;

int ToggleShopInventory(int tileIndex);
int DrawShop(bool is_in_shop);
void InitShopItems(void);
void UnloadShopItems(void);
void HandleShopItemClick(Vector2 mousePos, CombatState *combatState);
void UpdateShopItemsHover(Vector2 mousePos);

#endif