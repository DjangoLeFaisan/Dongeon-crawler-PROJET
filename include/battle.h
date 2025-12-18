#ifndef BATTLE_H
#define BATTLE_H

#include "raylib.h"

// Direction enum
typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

// États du chevalier
typedef enum {
    KNIGHT_IDLE,
    KNIGHT_ATTACKING,
    KNIGHT_DEFENDING
} KnightState;

// Structure du chevalier
typedef struct {
    int hp;
    int max_hp;
    int attack_power;
    int defense;
    KnightState state;
    float state_timer;
    Direction facing_direction;
    Rectangle attack_hitbox_front;
    Rectangle attack_hitbox_back;
    float attack_animation_timer;  // Timer pour l'animation d'attaque
    int attack_animation_frame;    // Frame actuel (0-3 pour les 4 sprites)
} Knight;

// Structure du Boss (simple, basé sur les orcs)
typedef struct {
    int gridX, gridY;
    float pixelX, pixelY;
    int hp, max_hp;
    int attack_power;
    bool is_alive;
    int texture_id;
    int facing_direction;  // 0=UP, 1=DOWN, 2=LEFT, 3=RIGHT
    float stun_timer;  // Timer pour étourdissement
} Boss;

// Structure globale du combat (mode overlay)
typedef struct {
    Knight knight;
    bool combat_overlay_active;  // Affiche les boutons sur la map
    Rectangle btn_attack;
    Rectangle btn_defend;
} CombatState;

// Fonctions
extern CombatState gCombatState;

void InitCombat(CombatState *state);
void UpdateCombat(CombatState *state, float dt);
void UpdateAttackHitboxesFromPlayer(CombatState *state, float playerPixelX, float playerPixelY);
void DrawCombat(const CombatState *state);
void ToggleCombatOverlay(void);
int choisirQuiCommence(void);

#endif
