#include "game.h"
#include "battle.h"
#include "raylib.h"
#include "marchand.h"
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define MAX_ENEMIES 16
#define ENEMY_SPAWN_ATTEMPTS 50
#define ENEMY_ATTACK_INTERVAL 0.7f  // Réduit de 1.0f à 0.7f pour attaquer plus souvent
#define ENEMY_ANIMATION_SPEED 350.0f  // pixels par seconde pour l'animation fluide (même que le joueur)
#define ENEMY_STUN_DURATION 1.0f  // Durée d'étourdissement après avoir été frappé

extern double defense_modifier;
extern double force_modifier;
extern int avarice_modifier;
extern int player_money;
extern Texture2D gTileTextures[];
extern int SOLID_TILES[];

// Vérifie si une tuile est solide
static bool IsSolidTile(int tileIndex)
{
    for (int i = 0; i < 99; i++) {
        if (SOLID_TILES[i] == tileIndex && SOLID_TILES[i] != 0) {
            return true;
        }
    }
    return false;
}

// Vérifie que la case est libre et dans la zone de jeu
static bool IsWalkable(const struct Board *board, int gridX, int gridY)
{
    if (gridX < 0 || gridX >= 34 || gridY < 0 || gridY >= (BOARD_ROWS - 2)) {
        return false;
    }

    const Tile *t = &board->tiles[gridY][gridX];
    if (t->layerCount <= 0) return true;
    int top = t->layers[t->layerCount - 1];
    return !IsSolidTile(top);
}

// Retourne la texture appropriée selon la direction
static int GetEnemyTextureForDirection(EnemyDirection dir)
{
    switch (dir) {
        case ENEMY_DIR_UP:    return 109;  // orcvertidldos
        case ENEMY_DIR_DOWN:  return 111;  // orcvertidlface
        case ENEMY_DIR_LEFT:  return 112;  // orcvertidlgauche
        case ENEMY_DIR_RIGHT: return 110;  // orcvertidldroite
        default: return 111;
    }
}

// Vérifie si une position est occupée par le joueur
static bool IsPlayerAt(const struct Board *board, int gridX, int gridY)
{
    return (board->player.gridX == gridX && board->player.gridY == gridY);
}

// Vérifie si une position est occupée par un autre ennemi
static bool IsEnemyAt(const struct Board *board, int gridX, int gridY, int excludeIndex)
{
    for (int i = 0; i < board->enemyCount; i++) {
        if (i == excludeIndex) continue;  // Ignorer l'ennemi lui-même
        if (board->enemies[i].is_alive && board->enemies[i].gridX == gridX && board->enemies[i].gridY == gridY) {
            return true;
        }
    }
    return false;
}

static void InitEnemy(Enemy *enemy, int gridX, int gridY)
{
    enemy->gridX = gridX;
    enemy->gridY = gridY;
    enemy->pixelX = (float)gridX * TILE_SIZE;
    enemy->pixelY = (float)gridY * TILE_SIZE;
    enemy->hp = 40;  // Augmenté de 30 à 40
    enemy->max_hp = 40;
    enemy->attack_power = 12;  // Augmenté de 8 à 12 pour infliger plus de dégâts
    enemy->attack_cooldown = 0.0f;
    enemy->was_hit_this_swing = false;
    enemy->is_alive = true;
    enemy->facing_direction = (EnemyDirection)GetRandomValue(0, 3);
    enemy->texture_id = GetEnemyTextureForDirection(enemy->facing_direction);
    enemy->movement_timer = 0.5f;
    enemy->think_timer = 0.0f;
    enemy->stun_timer = 0.0f;
    
    // Initialiser les hitboxes d'attaque
    enemy->attack_hitbox_front = (Rectangle){0, 0, 0, 0};
    enemy->attack_hitbox_back = (Rectangle){0, 0, 0, 0};
}

void ResetEnemies(struct Board *board)
{
    if (!board) return;
    board->enemyCount = 0;
    for (int i = 0; i < MAX_ENEMIES; i++) {
        board->enemies[i].is_alive = false;
    }
}

// Spawne 3 orcs sur les maps "Etage" (zone jouable 0-33)
void SpawnEnemiesForEtage(struct Board *board)
{
    if (!board) return;

    ResetEnemies(board);

    int spawned = 0;
    int attempts = 0;
    while (spawned < 3 && attempts < ENEMY_SPAWN_ATTEMPTS) {
        attempts++;
        int gx = GetRandomValue(2, 32);
        int gy = GetRandomValue(2, BOARD_ROWS - 4);

        if (!IsWalkable(board, gx, gy)) continue;

        InitEnemy(&board->enemies[spawned], gx, gy);
        spawned++;
    }

    board->enemyCount = spawned;
}

void ApplyDamageToEnemy(Enemy *enemy, int damage)
{
    if (!enemy || !enemy->is_alive) return;

    enemy->hp -= damage;
    if (enemy->hp <= 0) {
        enemy->hp = 0;
        enemy->is_alive = false;
        player_money += (5 * avarice_modifier);
    }
    
    // Appliquer l'étourdissement quand l'ennemi est frappé
    enemy->stun_timer = ENEMY_STUN_DURATION;
}

bool IsEnemyAlive(Enemy *enemy)
{
    return enemy && enemy->is_alive;
}

static Rectangle GetEnemyRect(const Enemy *enemy)
{
    return (Rectangle){enemy->pixelX, enemy->pixelY, TILE_SIZE, TILE_SIZE};
}

// Mise à jour des hitboxes d'attaque des ennemis (même que pour le joueur)
static void UpdateEnemyAttackHitboxes(Enemy *enemy) {
    double hitbox_width = 32;
    double hitbox_height = 32;
    float centerX = enemy->pixelX + (TILE_SIZE / 2);
    float centerY = enemy->pixelY + (TILE_SIZE / 2);
    
    // Hitbox devant l'ennemi selon sa direction
    switch (enemy->facing_direction) {
        case ENEMY_DIR_UP:
            enemy->attack_hitbox_front = (Rectangle){centerX - hitbox_width/2, enemy->pixelY - TILE_SIZE, hitbox_width, hitbox_height};
            enemy->attack_hitbox_back = (Rectangle){centerX - hitbox_width/2, enemy->pixelY + TILE_SIZE, hitbox_width, hitbox_height};
            break;
        case ENEMY_DIR_DOWN:
            enemy->attack_hitbox_front = (Rectangle){centerX - hitbox_width/2, enemy->pixelY + TILE_SIZE, hitbox_width, hitbox_height};
            enemy->attack_hitbox_back = (Rectangle){centerX - hitbox_width/2, enemy->pixelY - TILE_SIZE, hitbox_width, hitbox_height};
            break;
        case ENEMY_DIR_LEFT:
            enemy->attack_hitbox_front = (Rectangle){enemy->pixelX - TILE_SIZE, centerY - hitbox_height/2, hitbox_width, hitbox_height};
            enemy->attack_hitbox_back = (Rectangle){enemy->pixelX + TILE_SIZE, centerY - hitbox_height/2, hitbox_width, hitbox_height};
            break;
        case ENEMY_DIR_RIGHT:
            enemy->attack_hitbox_front = (Rectangle){enemy->pixelX + TILE_SIZE, centerY - hitbox_height/2, hitbox_width, hitbox_height};
            enemy->attack_hitbox_back = (Rectangle){enemy->pixelX - TILE_SIZE, centerY - hitbox_height/2, hitbox_width, hitbox_height};
            break;
    }
}

void UpdateEnemies(struct Board *board, float dt, CombatState *combatState)
{
    if (!board) return;

    Rectangle playerRect = {board->player.pixelX, board->player.pixelY, TILE_SIZE, TILE_SIZE};
    Rectangle frontHitbox = {0};
    Rectangle backHitbox = {0};

    if (combatState) {
        frontHitbox = combatState->knight.attack_hitbox_front;
        backHitbox = combatState->knight.attack_hitbox_back;
    }

    for (int i = 0; i < board->enemyCount; i++) {
        Enemy *e = &board->enemies[i];
        if (!e->is_alive) continue;

        e->attack_cooldown -= dt;
        if (e->attack_cooldown < 0.0f) e->attack_cooldown = 0.0f;

        // Gérer le timer d'étourdissement
        e->stun_timer -= dt;
        if (e->stun_timer < 0.0f) e->stun_timer = 0.0f;

        // ===== MOUVEMENT ET IA =====
        // Si l'ennemi est étourdi, ne pas bouger
        if (e->stun_timer > 0.0f) {
            // Continuer à mettre à jour les timers mais pas le mouvement
            e->movement_timer += dt;
            e->think_timer += dt;
        } else {
            e->movement_timer += dt;
            e->think_timer += dt;

            // Recalculer la direction vers le joueur tous les 0.5 secondes (ou plus souvent si proche)
            if (e->think_timer > 0.5f) {
                e->think_timer = 0.0f;
                
                // Calcul de la distance vers le joueur
                int distX = board->player.gridX - e->gridX;
                int distY = board->player.gridY - e->gridY;
                int absdistX = (distX < 0) ? -distX : distX;
                int absdistY = (distY < 0) ? -distY : distY;
                
                // Distance de détection = 15 cases
                if (absdistX + absdistY <= 15) {
                    // Le joueur est détecté - le poursuivre
                    if (absdistX > absdistY) {
                        // Mouvement horizontal prioritaire
                        e->facing_direction = (distX > 0) ? ENEMY_DIR_RIGHT : ENEMY_DIR_LEFT;
                    } else {
                        // Mouvement vertical prioritaire
                        e->facing_direction = (distY > 0) ? ENEMY_DIR_DOWN : ENEMY_DIR_UP;
                    }
                } else {
                    // Le joueur n'est pas détecté - patrouille aléatoire
                    if (GetRandomValue(0, 100) < 30) {  // 30% de chance de changer de direction
                        e->facing_direction = (EnemyDirection)GetRandomValue(0, 3);
                    }
                }
                
                e->texture_id = GetEnemyTextureForDirection(e->facing_direction);
            }

            // Déplacement plus rapide - toutes les 0.3 secondes au lieu de 0.5
            if (e->movement_timer > 0.3f) {
                e->movement_timer = 0.0f;
                
                int newGridX = e->gridX;
                int newGridY = e->gridY;

                // Calculer la nouvelle position selon la direction
                switch (e->facing_direction) {
                    case ENEMY_DIR_UP:
                        newGridY--;
                        break;
                    case ENEMY_DIR_DOWN:
                        newGridY++;
                        break;
                    case ENEMY_DIR_LEFT:
                        newGridX--;
                        break;
                    case ENEMY_DIR_RIGHT:
                        newGridX++;
                        break;
                }

                // Vérifier si la nouvelle position est praticable et non occupée
                if (IsWalkable(board, newGridX, newGridY) && 
                    !IsPlayerAt(board, newGridX, newGridY) && 
                    !IsEnemyAt(board, newGridX, newGridY, i)) {
                    e->gridX = newGridX;
                    e->gridY = newGridY;
                    e->pixelX = (float)e->gridX * TILE_SIZE;
                    e->pixelY = (float)e->gridY * TILE_SIZE;
                }
            }
        }

        Rectangle enemyRect = GetEnemyRect(e);

        // ===== ATTAQUE DIRECTE SI PROCHE DU JOUEUR =====
        // L'ennemi attaque aussi s'il est adjacent au joueur (même distance de grille)
        int distX = e->gridX - board->player.gridX;
        int distY = e->gridY - board->player.gridY;
        int absdistX = (distX < 0) ? -distX : distX;
        int absdistY = (distY < 0) ? -distY : distY;
        bool isAdjacentToPlayer = (absdistX + absdistY == 1);  // Directement adjacent (1 case de distance)

        // Attaque de l'ennemi s'il touche le joueur OU s'il est adjacent
        if (combatState && e->attack_cooldown <= 0.0f && (CheckCollisionRecs(enemyRect, playerRect) || isAdjacentToPlayer)) {
            // Vérifier si le joueur se défend et dans quelle direction
            bool blocked = false;
            if (combatState->knight.state == KNIGHT_DEFENDING) {
                // Calculer la direction de l'ennemi par rapport au joueur
                int distX = e->gridX - board->player.gridX;
                int distY = e->gridY - board->player.gridY;
                
                // Déterminer de quel côté l'ennemi attaque
                Direction attackDirection = DIR_DOWN;
                if (distX < 0 && distX * distX > distY * distY) {
                    // Ennemi à droite du joueur -> attaque de la droite
                    attackDirection = DIR_RIGHT;
                } else if (distX > 0 && distX * distX > distY * distY) {
                    // Ennemi à gauche du joueur -> attaque de la gauche
                    attackDirection = DIR_LEFT;
                } else if (distY < 0) {
                    // Ennemi en bas du joueur -> attaque d'en bas
                    attackDirection = DIR_DOWN;
                } else {
                    // Ennemi en haut du joueur -> attaque d'en haut
                    attackDirection = DIR_UP;
                }
                
                // Si le joueur se défend dans la direction de l'attaque, l'attaque est bloquée
                if (combatState->knight.facing_direction == attackDirection) {
                    blocked = true;
                    TraceLog(LOG_INFO, "Attaque bloquée! Le joueur se défend dans la direction correcte!");
                }
            }
            
            // Infliger les dégâts seulement si l'attaque n'est pas bloquée
            if (!blocked) {
                combatState->knight.hp -= (e->attack_power *= defense_modifier);
                if (combatState->knight.hp < 0) combatState->knight.hp = 0;
                TraceLog(LOG_INFO, "Ennemi attaque le chevalier! HP: %d", combatState->knight.hp);
            }
            
            e->attack_cooldown = ENEMY_ATTACK_INTERVAL;
        }

        // Le chevalier inflige des dégâts quand il attaque et que la hitbox touche l'ennemi
        if (combatState && combatState->knight.state == KNIGHT_ATTACKING) {
            bool hitFront = CheckCollisionRecs(enemyRect, frontHitbox);
            bool hitBack = CheckCollisionRecs(enemyRect, backHitbox);
            if ((hitFront || hitBack) && !e->was_hit_this_swing) {
                int knight_attack_power = combatState->knight.attack_power * force_modifier;
                ApplyDamageToEnemy(e, knight_attack_power);
                e->was_hit_this_swing = true;
                TraceLog(LOG_INFO, "Ennemi touché! HP restant: %d", e->hp);
            }
        } else {
            // Réinitialiser le flag une fois l'attaque terminée
            e->was_hit_this_swing = false;
        }

        // ===== ANIMATION FLUIDE =====
        // Animation fluide vers la position cible (comme le joueur)
        float targetPixelX = e->gridX * TILE_SIZE;
        float targetPixelY = e->gridY * TILE_SIZE;
        
        float diffX = targetPixelX - e->pixelX;
        float diffY = targetPixelY - e->pixelY;
        
        float moveAmount = ENEMY_ANIMATION_SPEED * dt;
        
        // Anime pixelX
        if (fabsf(diffX) > 0.5f) {
            if (fabsf(diffX) <= moveAmount) {
                e->pixelX = targetPixelX;
            } else {
                e->pixelX += (diffX > 0 ? moveAmount : -moveAmount);
            }
        }
        
        // Anime pixelY
        if (fabsf(diffY) > 0.5f) {
            if (fabsf(diffY) <= moveAmount) {
                e->pixelY = targetPixelY;
            } else {
                e->pixelY += (diffY > 0 ? moveAmount : -moveAmount);
            }
        }
        
        // Mettre à jour les hitboxes d'attaque selon la position et la direction actuelles
        UpdateEnemyAttackHitboxes(e);
    }
}

void DrawEnemies(const struct Board *board)
{
    if (!board) return;

    for (int i = 0; i < board->enemyCount; i++) {
        const Enemy *e = &board->enemies[i];
        if (!e->is_alive) continue;

        Texture2D tex = {0};
        if (e->texture_id >= 0) {
            tex = gTileTextures[e->texture_id];
        }

        if (tex.id > 0) {
            DrawTexture(tex, (int)e->pixelX, (int)e->pixelY, WHITE);
        } else {
            DrawRectangle((int)e->pixelX, (int)e->pixelY, TILE_SIZE, TILE_SIZE, DARKGREEN);
        }
        
        // Afficher la hitbox de l'ennemi en rouge
        Rectangle enemyHitbox = {
            (int)e->pixelX,
            (int)e->pixelY,
            TILE_SIZE,
            TILE_SIZE
        };
        DrawRectangleRec(enemyHitbox, (Color){255, 0, 0, 30});  // Rouge très transparent
        DrawRectangleLinesEx(enemyHitbox, 1, (Color){255, 0, 0, 150});  // Bordure rouge
        
        // Afficher les hitboxes d'attaque de l'ennemi (comme le joueur)
        DrawRectangleRec(e->attack_hitbox_front, (Color){255, 0, 0, 100});  // Rouge semi-transparent
        DrawRectangleLinesEx(e->attack_hitbox_front, 2, RED);  // Bordure rouge
        
        DrawRectangleRec(e->attack_hitbox_back, (Color){255, 0, 0, 50});   // Rouge très transparent
        DrawRectangleLinesEx(e->attack_hitbox_back, 1, (Color){255, 0, 0, 150});

        // Barre de vie
        int bar_width = TILE_SIZE;
        int bar_height = 4;
        int hp_percent = (e->hp * 100) / e->max_hp;
        DrawRectangle((int)e->pixelX, (int)e->pixelY - 6, bar_width, bar_height, RED);
        DrawRectangle((int)e->pixelX, (int)e->pixelY - 6,
                      (bar_width * hp_percent) / 100, bar_height, GREEN);
    }
}