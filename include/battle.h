#ifndef BATTLE_H
#define BATTLE_H

#include "raylib.h"

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
