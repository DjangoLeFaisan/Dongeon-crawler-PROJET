#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "battle.h"

// Direction enum (même que le chevalier)
typedef enum {
    ENEMY_DIR_UP,
    ENEMY_DIR_DOWN,
    ENEMY_DIR_LEFT,
    ENEMY_DIR_RIGHT
} EnemyDirection;

// Définition des ennemis (orc vert pour le moment)
typedef struct {
    // Position grille et pixels
    int gridX;
    int gridY;
    float pixelX;
    float pixelY;

    // Stats de combat
    int hp;
    int max_hp;
    int attack_power;

    // Timers
    float attack_cooldown;   // temps avant prochaine attaque
    bool was_hit_this_swing; // évite de prendre plusieurs fois la même attaque
    float movement_timer;    // timer pour mouvement et IA
    float think_timer;       // timer pour changer de direction
    float stun_timer;        // timer d'étourdissement après avoir été frappé

    // État
    bool is_alive;
    EnemyDirection facing_direction;

    // Texture
    int texture_id;          // index dans gTileTextures (direction face)
} Enemy;

// Forward declarations pour éviter les inclusions circulaires  
// Utilisation de 'struct Board' au lieu de 'Board' pour éviter les conflits de typedef
struct Board;

// Gestion des ennemis
void ResetEnemies(struct Board *board);
void SpawnEnemiesForEtage(struct Board *board);
void UpdateEnemies(struct Board *board, float dt, CombatState *combatState);
void DrawEnemies(const struct Board *board);
void ApplyDamageToEnemy(Enemy *enemy, int damage);
bool IsEnemyAlive(Enemy *enemy);

#endif
