#include "marcher.h"
#include "game.h"
#include "marchand.h"
#include "raylib.h"
#include <math.h>

extern double speed_modifier;
extern int SOLID_TILES[];
#define SOLID_TILES_COUNT 99
extern bool special_level;
extern bool LoadNextLevel(Board *board);

static float moveTimer = 0.0f;

// Vitesses de base - ce sont tes valeurs "normales"
static const float BASE_MOVE_DELAY = 0.2f;        // Délai de base entre les mouvements
static const float BASE_ANIMATION_SPEED = 350.0f;  // Vitesse d'animation de base

bool can_player_move = false;

// Fonction pour vérifier si une tuile est solide
bool VerifySolidTile(int tileIndex)
{
    for (int x = 0; x < SOLID_TILES_COUNT; x++) {
        if ((SOLID_TILES[x] == tileIndex) && (SOLID_TILES[x] != 0)) {
            return true;
        }
    }
    return false;
}

// Fonction pour récupérer l'index de tuile aux coordonnées données
int GetTileAtGridPos(const Board *board, int gridX, int gridY)
{
    if (gridX < 0 || gridX >= BOARD_COLS || gridY < 0 || gridY >= BOARD_ROWS) {
        return -1;
    }
    
    const Tile *t = &board->tiles[gridY][gridX];
    
    if (t->layerCount > 0) {
        return t->layers[t->layerCount - 1];
    }
    
    return -1;
}

void Marcher(Player *player, Board *board) 
{
    // Calcule les vitesses modifiées en fonction du modificateur
    // Plus speed_modifier est grand, plus le joueur va vite
    float currentMoveDelay = BASE_MOVE_DELAY / speed_modifier;
    float currentAnimSpeed = BASE_ANIMATION_SPEED * speed_modifier;
    
    moveTimer -= GetFrameTime();
    
    int dx = 0;
    int dy = 0;

    if (IsKeyDown(KEY_W)) {       
        dy = -1;
    }
    if (IsKeyDown(KEY_S)) {
        dy = 1;
    }
    if (IsKeyDown(KEY_A)) {
        dx = -1;
    }
    if (IsKeyDown(KEY_D)) {
        dx = 1;
    }

    if ((dx != 0 || dy != 0) && moveTimer <= 0.0f) {
        can_player_move = true;
        
        if (dx != 0) {
            player->lastDirection = dx > 0 ? 1 : -1;
        }

        int newX = player->gridX + dx;
        int newY = player->gridY + dy;
        
        if (newX < 0 || newX >= 34 || newY < 0 || newY >= (BOARD_ROWS - 2)) {
            can_player_move = false;
        }
        
        int tileIndex = GetTileAtGridPos(board, newX, newY);

        ToggleShopInventory(tileIndex);
        
        if (tileIndex >= 0 && VerifySolidTile(tileIndex)) {
            can_player_move = false;
        } else if (tileIndex == 28) {
            LoadNextLevel(board);
            return;
        }
        
        if (can_player_move) {
            player->gridX = newX;
            player->gridY = newY;
            
            if (dx != 0) {
                player->lastDirection = (dx > 0) ? 1 : -1;
            }
            
            TraceLog(LOG_DEBUG, "Player moved to grid position (%d, %d)", newX, newY);
            
            // Utilise le délai modifié au lieu de la constante
            moveTimer = currentMoveDelay;
        }
    }
    
    // Animation fluide vers la position cible avec la vitesse modifiée
    float targetPixelX = player->gridX * TILE_SIZE;
    float targetPixelY = player->gridY * TILE_SIZE;
    
    float diffX = targetPixelX - player->pixelX;
    float diffY = targetPixelY - player->pixelY;
    
    // Utilise la vitesse d'animation modifiée
    float moveAmount = currentAnimSpeed * GetFrameTime();
    
    // Anime pixelX
    if (fabsf(diffX) > 0.5f) {
        if (fabsf(diffX) <= moveAmount) {
            player->pixelX = targetPixelX;
        } else {
            player->pixelX += (diffX > 0 ? moveAmount : -moveAmount);
        }
    }
    
    // Anime pixelY
    if (fabsf(diffY) > 0.5f) {
        if (fabsf(diffY) <= moveAmount) {
            player->pixelY = targetPixelY;
        } else {
            player->pixelY += (diffY > 0 ? moveAmount : -moveAmount);
        }
    }
}