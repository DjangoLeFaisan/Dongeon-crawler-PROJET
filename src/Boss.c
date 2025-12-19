#include "game.h"
#include "battle.h"
#include "raylib.h"
#include <math.h>
#include <string.h>

extern Texture2D gTileTextures[];
extern int current_level;
extern Sound gDeathSound;

// Constantes de direction
#define ENEMY_DIR_UP 0
#define ENEMY_DIR_DOWN 1
#define ENEMY_DIR_LEFT 2
#define ENEMY_DIR_RIGHT 3

// Boss global (variable statique)
static Boss gBoss = {0};
static bool gBossSpawned = false;
bool gBossActive = false;
static bool gBossWasHitThisSwing = false;  // Flag pour éviter les dégâts multiples
static float gBossAttackCooldown = 0.0f;   // Cooldown avant prochaine attaque
static Rectangle gBossAttackHitboxFront = {0};
static Rectangle gBossAttackHitboxBack = {0};
static bool gBossAlerted = false;  // Flag : le boss a-t-il détecté le joueur ?

// Retourne la texture du boss selon sa direction
static int GetBossTextureForDirection(int dir)
{
    switch (dir) {
        case ENEMY_DIR_UP:    return 114;  // BOSSderriere
        case ENEMY_DIR_DOWN:  return 113;  // BOSSdevant
        case ENEMY_DIR_LEFT:  return 115;  // BOSSgauche
        case ENEMY_DIR_RIGHT: return 116;  // BOSSdroite
        default: return 113;
    }
}

// Mettre à jour les hitboxes d'attaque du boss selon sa direction
static void UpdateBossAttackHitboxes(void) {
    double hitbox_width = 32;
    double hitbox_height = 32;
    float centerX = gBoss.pixelX + (TILE_SIZE / 2);
    float centerY = gBoss.pixelY + (TILE_SIZE / 2);
    
    switch (gBoss.facing_direction) {
        case ENEMY_DIR_UP:
            gBossAttackHitboxFront = (Rectangle){centerX - hitbox_width/2, gBoss.pixelY - TILE_SIZE, hitbox_width, hitbox_height};
            gBossAttackHitboxBack = (Rectangle){centerX - hitbox_width/2, gBoss.pixelY + TILE_SIZE, hitbox_width, hitbox_height};
            break;
        case ENEMY_DIR_DOWN:
            gBossAttackHitboxFront = (Rectangle){centerX - hitbox_width/2, gBoss.pixelY + TILE_SIZE, hitbox_width, hitbox_height};
            gBossAttackHitboxBack = (Rectangle){centerX - hitbox_width/2, gBoss.pixelY - TILE_SIZE, hitbox_width, hitbox_height};
            break;
        case ENEMY_DIR_LEFT:
            gBossAttackHitboxFront = (Rectangle){gBoss.pixelX - TILE_SIZE, centerY - hitbox_height/2, hitbox_width, hitbox_height};
            gBossAttackHitboxBack = (Rectangle){gBoss.pixelX + TILE_SIZE, centerY - hitbox_height/2, hitbox_width, hitbox_height};
            break;
        case ENEMY_DIR_RIGHT:
            gBossAttackHitboxFront = (Rectangle){gBoss.pixelX + TILE_SIZE, centerY - hitbox_height/2, hitbox_width, hitbox_height};
            gBossAttackHitboxBack = (Rectangle){gBoss.pixelX - TILE_SIZE, centerY - hitbox_height/2, hitbox_width, hitbox_height};
            break;
    }
}

// Spawn le boss une seule fois par étage (seulement sur Etage8)
void SpawnBoss(Board *board, const char *mapName) {
    if (gBossSpawned) return;
    
    // Le boss n'apparaît que sur Etage9
    if (mapName == NULL || strstr(mapName, "Etage9") == NULL) {
        return;
    }
    
    // Le boss apparaît au centre de l'étoile sur Etage9
    gBoss.gridX = 18;
    gBoss.gridY = 8;
    gBoss.pixelX = gBoss.gridX * TILE_SIZE;
    gBoss.pixelY = gBoss.gridY * TILE_SIZE;
    gBoss.hp = 300;
    gBoss.max_hp = 300;
    gBoss.attack_power = 15;
    gBoss.is_alive = true;
    gBoss.facing_direction = ENEMY_DIR_DOWN;
    gBoss.texture_id = GetBossTextureForDirection(gBoss.facing_direction);
    gBoss.stun_timer = 0.0f;
    gBossSpawned = true;
    gBossActive = true;
    TraceLog(LOG_INFO, "BOSS SPAWNED at (%d, %d) on Etage9!", gBoss.gridX, gBoss.gridY);
}

// Mise à jour du boss avec déplacement fluide et IA
void UpdateBoss(Board *board, float dt) {
    if (!gBossActive || !gBoss.is_alive) return;
    
    // Mettre à jour le timer d'étourdissement
    if (gBoss.stun_timer > 0.0f) {
        gBoss.stun_timer -= dt;
        if (gBoss.stun_timer < 0.0f) gBoss.stun_timer = 0.0f;
    }
    
    // Mettre à jour le cooldown d'attaque
    if (gBossAttackCooldown > 0.0f) {
        gBossAttackCooldown -= dt;
    }
    
    // Déplacement fluide du boss
    float targetX = gBoss.gridX * TILE_SIZE;
    float targetY = gBoss.gridY * TILE_SIZE;
    float dx = targetX - gBoss.pixelX;
    float dy = targetY - gBoss.pixelY;
    float dist = sqrt(dx*dx + dy*dy);
    
    // Mettre à jour la direction selon le mouvement
    if (dist > 1.0f) {
        if (fabs(dx) > fabs(dy)) {
            if (dx > 0) {
                gBoss.facing_direction = ENEMY_DIR_RIGHT;
            } else {
                gBoss.facing_direction = ENEMY_DIR_LEFT;
            }
        } else {
            if (dy > 0) {
                gBoss.facing_direction = ENEMY_DIR_DOWN;
            } else {
                gBoss.facing_direction = ENEMY_DIR_UP;
            }
        }
    }
    
    // Déplacement fluide en pixels (350 px/s comme le joueur)
    if (dist > 0.5f) {
        float speed = 450.0f * dt;  // Augmenté de 350 à 450 px/s
        if (dist < speed) {
            gBoss.pixelX = targetX;
            gBoss.pixelY = targetY;
        } else {
            gBoss.pixelX += (dx / dist) * speed;
            gBoss.pixelY += (dy / dist) * speed;
        }
    }
    
    // IA du boss: se déplacer vers le joueur (seulement s'il n'est pas étourdi ET seulement si le joueur est proche)
    if (gBoss.stun_timer <= 0.0f) {
        static float thinkTimer = 0.0f;
        thinkTimer += dt;
        
        if (thinkTimer >= 0.8f) {  // Réduit de 1.5f à 0.8f pour plus rapide
            thinkTimer = 0.0f;
            
            int playerGridX = board->player.gridX;
            int playerGridY = board->player.gridY;
            int diffX = playerGridX - gBoss.gridX;
            int diffY = playerGridY - gBoss.gridY;
            
            // Calculer la distance Manhattan vers le joueur
            int distance = abs(diffX) + abs(diffY);
            
            // Si le joueur se rapproche à moins de 6 cases, le boss est alerté
            if (distance < 6) {
                gBossAlerted = true;
            }
            
            // Le boss bouge une fois qu'il est alerté
            if (gBossAlerted) {
                // Se rapprocher du joueur avec limites de la map
                int newGridX = gBoss.gridX;
                int newGridY = gBoss.gridY;
                
                if (abs(diffX) > abs(diffY)) {
                    if (diffX > 0 && gBoss.gridX < BOARD_COLS - 2) newGridX++;
                    else if (diffX < 0 && gBoss.gridX > 1) newGridX--;
                } else {
                    if (diffY > 0 && gBoss.gridY < BOARD_ROWS - 3) newGridY++;
                    else if (diffY < 0 && gBoss.gridY > 1) newGridY--;
                }
                
                // Vérifier la collision avec le joueur
                if (newGridX != board->player.gridX || newGridY != board->player.gridY) {
                    gBoss.gridX = newGridX;
                    gBoss.gridY = newGridY;
                }
            }
        }
    }
    
    // Boss attaque le joueur
    BossAttackPlayer(board, gBoss.attack_power);
    
    // Mettre à jour les hitboxes d'attaque
    UpdateBossAttackHitboxes();
}

// Afficher le boss
void DrawBoss(const Board *board) {
    if (!gBossActive || !gBoss.is_alive) return;
    
    // Mettre à jour la texture selon la direction
    gBoss.texture_id = GetBossTextureForDirection(gBoss.facing_direction);
    
    DrawTexture(gTileTextures[gBoss.texture_id], (int)gBoss.pixelX, (int)gBoss.pixelY, WHITE);
    
    // Afficher la barre de vie du boss au-dessus de lui
    int barWidth = 60;
    int barHeight = 8;
    int barX = (int)gBoss.pixelX + (TILE_SIZE - barWidth) / 2;
    int barY = (int)gBoss.pixelY - 15;
    
    // Fond de la barre (gris)
    DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
    
    // Barre de vie (rouge/vert selon santé)
    int hpBarWidth = (int)(barWidth * ((float)gBoss.hp / (float)gBoss.max_hp));
    Color hpColor = gBoss.hp > gBoss.max_hp / 2 ? GREEN : RED;
    DrawRectangle(barX, barY, hpBarWidth, barHeight, hpColor);
    
    // Bordure
    DrawRectangleLines(barX, barY, barWidth, barHeight, WHITE);
}

// Réinitialiser le boss
void ResetBoss(void) {
    gBossSpawned = false;
    gBossActive = false;
    gBoss.is_alive = false;
    gBossAlerted = false;  // Réinitialiser le flag d'alerte
}

// Vérifier les collisions et infliger des dégâts au boss
bool CheckBossCollision(int gridX, int gridY) {
    if (!gBossActive || !gBoss.is_alive) return false;
    return (gBoss.gridX == gridX && gBoss.gridY == gridY);
}

// Infliger des dégâts au boss
void DamageBoss(int damage) {
    if (!gBossActive || !gBoss.is_alive) return;
    gBoss.hp -= damage;
    if (gBoss.hp <= 0) {
        gBoss.hp = 0;
        gBoss.is_alive = false;
        gBossActive = false;
        
        // Jouer le son de mort
        if (gDeathSound.frameCount > 0) {
            PlaySound(gDeathSound);
        }
    }
}

// Attaquer le joueur
void BossAttackPlayer(Board *board, int damage) {
    if (!gBossActive || !gBoss.is_alive) return;
    
    // Vérifier la distance avec le joueur (1 case)
    int distX = abs(gBoss.gridX - board->player.gridX);
    int distY = abs(gBoss.gridY - board->player.gridY);
    
    if (distX <= 1 && distY <= 1 && gBossAttackCooldown <= 0.0f) {
        extern CombatState gCombatState;
        
        // Vérifier si le joueur se défend et dans quelle direction
        bool blocked = false;
        if (gCombatState.knight.state == KNIGHT_DEFENDING) {
            // Calculer la direction du boss par rapport au joueur
            int diffX = gBoss.gridX - board->player.gridX;
            int diffY = gBoss.gridY - board->player.gridY;
            
            // Déterminer de quel côté le boss attaque
            Direction attackDirection = DIR_DOWN;
            if (diffX < 0 && diffX * diffX > diffY * diffY) {
                // Boss à gauche du joueur -> attaque de la gauche
                attackDirection = DIR_LEFT;
            } else if (diffX > 0 && diffX * diffX > diffY * diffY) {
                // Boss à droite du joueur -> attaque de la droite
                attackDirection = DIR_RIGHT;
            } else if (diffY < 0 && diffY * diffY > diffX * diffX) {
                // Boss en haut du joueur -> attaque d'en haut
                attackDirection = DIR_UP;
            } else if (diffY > 0 && diffY * diffY > diffX * diffX) {
                // Boss en bas du joueur -> attaque d'en bas
                attackDirection = DIR_DOWN;
            }
            
            // Vérifier si l'attaque est dans la zone de défense
            bool isInDefenseZone = false;
            
            switch (gCombatState.knight.facing_direction) {
                case DIR_RIGHT:
                    if (attackDirection == DIR_RIGHT || attackDirection == DIR_UP || attackDirection == DIR_DOWN) {
                        isInDefenseZone = true;
                    }
                    break;
                case DIR_LEFT:
                    if (attackDirection == DIR_LEFT || attackDirection == DIR_UP || attackDirection == DIR_DOWN) {
                        isInDefenseZone = true;
                    }
                    break;
                case DIR_UP:
                    if (attackDirection == DIR_UP || attackDirection == DIR_LEFT || attackDirection == DIR_RIGHT) {
                        isInDefenseZone = true;
                    }
                    break;
                case DIR_DOWN:
                    if (attackDirection == DIR_DOWN || attackDirection == DIR_LEFT || attackDirection == DIR_RIGHT) {
                        isInDefenseZone = true;
                    }
                    break;
            }
            
            if (isInDefenseZone) {
                blocked = true;
                gBoss.stun_timer = 3.0f;  // Étourdir le boss pendant 3 secondes
                
                // Jouer le son de bloc
                extern Sound gBlockSound;
                if (gBlockSound.frameCount > 0) {
                    PlaySound(gBlockSound);
                }
                
                TraceLog(LOG_INFO, "Attaque du Boss bloquée! Le joueur se défend!");
            }
        }
        
        // Infliger les dégâts seulement si l'attaque n'est pas bloquée
        if (!blocked) {
            gCombatState.knight.hp -= damage;
            if (gCombatState.knight.hp < 0) gCombatState.knight.hp = 0;
            
            // Jouer le son d'attaque du boss
            extern Sound gEnemyAttackSound;
            if (gEnemyAttackSound.frameCount > 0) {
                PlaySound(gEnemyAttackSound);
            }
            
            TraceLog(LOG_INFO, "Boss attaque! Joueur HP: %d/%d", gCombatState.knight.hp, gCombatState.knight.max_hp);
        }
        
        gBossAttackCooldown = 1.2f;  // Cooldown de 1.2 secondes
    }
}

// Obtenir le boss pour pouvoir vérifier les collisions
Boss* GetBoss(void) {
    return &gBoss;
}

// Vérifier si le boss est actif
bool GetBossActive(void) {
    return gBossActive;
}

// Vérifier si le boss a été touché cette frame
bool CheckBossHitThisSwing(void) {
    return gBossWasHitThisSwing;
}

// Définir si le boss a été touché cette frame
void SetBossHitThisSwing(bool value) {
    gBossWasHitThisSwing = value;
}