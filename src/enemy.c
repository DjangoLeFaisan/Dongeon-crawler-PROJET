/*
#include "enemy.h"
#include <stdlib.h>

// Crée un ennemi
Enemy* CreateEnemy(int gridX, int gridY, int hp, int attack) {
    Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));
    if (!enemy) return NULL;
    
    enemy->gridX = gridX;
    enemy->gridY = gridY;
    enemy->pixelX = (float)gridX * 32;
    enemy->pixelY = (float)gridY * 32;
    
    enemy->hp = hp;
    enemy->max_hp = hp;
    enemy->attack_power = attack;
    
    enemy->is_alive = true;
    enemy->texture.id = 0;
    enemy->texture_id = -1;
    
    return enemy;
}

// Détruit un ennemi
void DestroyEnemy(Enemy* enemy) {
    if (enemy) {
        if (enemy->texture.id != 0) {
            UnloadTexture(enemy->texture);
        }
        free(enemy);
    }
}

// Affiche un ennemi
void DrawEnemy(const Enemy* enemy) {
    if (!enemy || !enemy->is_alive) return;
    
    if (enemy->texture.id != 0) {
        DrawTexture(enemy->texture, (int)enemy->pixelX, (int)enemy->pixelY, WHITE);
    } else {
        // Carré rouge par défaut
        DrawRectangle((int)enemy->pixelX, (int)enemy->pixelY, 32, 32, RED);
    }
    
    // Barre de vie
    int bar_width = 32;
    int bar_height = 4;
    int hp_percent = (enemy->hp * 100) / enemy->max_hp;
    
    DrawRectangle((int)enemy->pixelX, (int)enemy->pixelY - 6, bar_width, bar_height, RED);
    DrawRectangle((int)enemy->pixelX, (int)enemy->pixelY - 6, 
                 (bar_width * hp_percent) / 100, bar_height, GREEN);
}

// Applique des dégâts à l'ennemi
void ApplyDamageToEnemy(Enemy* enemy, int damage) {
    if (!enemy) return;
    
    enemy->hp -= damage;
    if (enemy->hp <= 0) {
        enemy->hp = 0;
        enemy->is_alive = false;
    }
}

// Vérifie si l'ennemi est vivant
bool IsEnemyAlive(Enemy* enemy) {
    return enemy && enemy->is_alive;
}
*/