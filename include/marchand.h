#ifndef MARCHAND
#define MARCHAND

int ToggleShopInventory(int tileIndex);
int DrawShop(bool is_in_shop);
void InitShopItems(void);
void UnloadShopItems(void);
void HandleShopItemClick(Vector2 mousePos, int player_money);
void UpdateShopItemsHover(Vector2 mousePos);

#endif