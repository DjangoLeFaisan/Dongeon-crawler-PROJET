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
void DrawCombat(const CombatState *state);
void ToggleCombatOverlay(void);
int choisirQuiCommence(void);

#endif
