#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

// Structure simple d'un ennemi
typedef struct {
    // Position
    int gridX;
    int gridY;
    float pixelX;
    float pixelY;
    
    // Stats de combat
    int hp;
    int max_hp;
    int attack_power;
    
    // État
    bool is_alive;
    
    // Texture et affichage
    Texture2D texture;
    int texture_id;
} Enemy;

// Fonctions
Enemy* CreateEnemy(int gridX, int gridY, int hp, int attack);
void DestroyEnemy(Enemy* enemy);
void DrawEnemy(const Enemy* enemy);
void ApplyDamageToEnemy(Enemy* enemy, int damage);
bool IsEnemyAlive(Enemy* enemy);
