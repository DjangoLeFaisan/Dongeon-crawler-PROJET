#include "marcher.h"
#include "game.h"
#include "raylib.h"
#include <math.h>

extern int SOLID_TILES[];
#define SOLID_TILES_COUNT 99  // Nombre de tuiles solides
extern bool special_level;  // Déclaration externe pour accéder à la variable de level_connexion.c
extern bool LoadNextLevel(Board *board);  // Déclaration de la fonction

static float moveTimer = 0.0f;
static const float MOVE_DELAY = 0.2f;
static const float ANIMATION_SPEED = 350.0f;  // pixels par seconde pour l'animation fluide

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
    //Si hors limites
    if (gridX < 0 || gridX >= BOARD_COLS || gridY < 0 || gridY >= BOARD_ROWS) {
        return -1;
    }
    
    const Tile *t = &board->tiles[gridY][gridX];
    
    // Retourne la tuile visible la plus haute
    if (t->layerCount > 0) {
        return t->layers[t->layerCount - 1];
    }
    
    return -1;
}

void Marcher(Player *player, const Board *board) 
{
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

        // Calcule la nouvelle position
        int newX = player->gridX + dx;
        int newY = player->gridY + dy;
        
        // Vérifie les limites de la grille
        if (newX < 0 || newX >= 34 || newY < 0 || newY >= (BOARD_ROWS - 2)) {
            can_player_move = false;
        }
        
        // Récupère la tuile à la nouvelle position
        int tileIndex = GetTileAtGridPos(board, newX, newY);
        
        // Vérifie si la tuile est solide
        if (tileIndex >= 0 && VerifySolidTile(tileIndex)) {
            can_player_move = false;
        } else if (tileIndex == 28) {
            special_level = true;  
            LoadNextLevel(board);
        }
        // Mouvement autorisé
        if (can_player_move) {
            player->gridX = newX;
            player->gridY = newY;
            TraceLog(LOG_DEBUG, "Player moved to grid position (%d, %d)", newX, newY);
            moveTimer = MOVE_DELAY;
        }
    }
    
    // Animation fluide vers la position cible
    float targetPixelX = player->gridX * TILE_SIZE;
    float targetPixelY = player->gridY * TILE_SIZE;
    
    float diffX = targetPixelX - player->pixelX;
    float diffY = targetPixelY - player->pixelY;
    
    float moveAmount = ANIMATION_SPEED * GetFrameTime();
    
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