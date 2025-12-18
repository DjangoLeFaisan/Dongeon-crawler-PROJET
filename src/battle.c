// Système de combat en overlay
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "game.h"
#include "battle.h"

#define ACTION_DURATION 1.0f
#define DEFEND_REDUCTION 0.5f

double attack_power = 10;
double hitbox_width = 32;
double hitbox_height = 32;

extern double attack_speed_modifier;
extern double range_modifier;

extern bool editor_active;
extern bool is_in_shop;

// Variable globale du combat
CombatState gCombatState = {0};

// Vérifie si la souris survole un bouton
static bool IsButtonHovered(Rectangle btn) {
    return CheckCollisionPointRec(GetMousePosition(), btn);
}

// Met à jour la hitbox d'attaque selon la direction
static void UpdateAttackHitboxes(Knight *knight, float playerPixelX, float playerPixelY) {
    switch (knight->facing_direction) {
        case DIR_UP:
            knight->attack_hitbox_front = (Rectangle){playerPixelX - TILE_SIZE, playerPixelY - (range_modifier * TILE_SIZE), 3*TILE_SIZE, TILE_SIZE * range_modifier};
            break;
        case DIR_DOWN:
            knight->attack_hitbox_front = (Rectangle){playerPixelX - TILE_SIZE, playerPixelY + TILE_SIZE, 3*TILE_SIZE, TILE_SIZE * range_modifier};
            break;
        case DIR_LEFT:
            knight->attack_hitbox_front = (Rectangle){playerPixelX - (range_modifier * TILE_SIZE), playerPixelY - TILE_SIZE, TILE_SIZE * range_modifier, 3*TILE_SIZE};
            break;
        case DIR_RIGHT:
            knight->attack_hitbox_front = (Rectangle){playerPixelX + TILE_SIZE, playerPixelY - TILE_SIZE, TILE_SIZE * range_modifier, 3*TILE_SIZE};
            break;
    }
}

// Initialise l'état du combat
void InitCombat(CombatState *state) {
    state->knight.hp = 100;
    state->knight.max_hp = 100;
    state->knight.attack_power = attack_power;
    state->knight.defense = 5;
    state->knight.state = KNIGHT_IDLE;
    state->knight.state_timer = 0;
    state->knight.facing_direction = DIR_DOWN;
    state->knight.attack_animation_timer = 0;
    state->knight.attack_animation_frame = 0;
    
    state->knight.attack_hitbox_front = (Rectangle){0, 0, 0, 0};
    state->knight.attack_hitbox_back = (Rectangle){0, 0, 0, 0};

    state->btn_attack = (Rectangle){500, 650, 140, 50};
    state->btn_defend = (Rectangle){640, 650, 140, 50};

    state->combat_overlay_active = false;
}

// Met à jour l'état du combat à chaque frame
void UpdateCombat(CombatState *state, float dt) {
    if (!state->combat_overlay_active) return;

    // SECTION 1: Mise à jour de la direction
    Direction newDirection = state->knight.facing_direction;
    if (IsKeyDown(KEY_W)) {
        newDirection = DIR_UP;
    } else if (IsKeyDown(KEY_S)) {
        newDirection = DIR_DOWN;
    } else if (IsKeyDown(KEY_A)) {
        newDirection = DIR_LEFT;
    } else if (IsKeyDown(KEY_D)) {
        newDirection = DIR_RIGHT;
    }
    
    if (newDirection != state->knight.facing_direction) {
        state->knight.facing_direction = newDirection;
    }

    // SECTION 2: Gestion des timers
    if (state->knight.state_timer > 0) {
        state->knight.state_timer -= dt;
        if (state->knight.state_timer <= 0) {
            state->knight.state = KNIGHT_IDLE;
        }
    }
    
    // SECTION 3: Animation d'attaque
    if (state->knight.state == KNIGHT_ATTACKING) {
        state->knight.attack_animation_timer += dt;
        state->knight.attack_animation_frame = (int)(state->knight.attack_animation_timer / 0.1f);
        
        if (state->knight.attack_animation_frame > 3) {
            state->knight.attack_animation_frame = 3;
            state->knight.state = KNIGHT_IDLE;
            state->knight.attack_animation_timer = 0;
        }
    } else {
        state->knight.attack_animation_timer = 0;
        state->knight.attack_animation_frame = 0;
    }

    // SECTION 4: Entrées utilisateur (attaque/défense)
    if (state->knight.state == KNIGHT_IDLE) {
        if (IsKeyPressed(KEY_KP_4) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state->knight.state = KNIGHT_ATTACKING;
            state->knight.state_timer = (ACTION_DURATION * attack_speed_modifier);
            state->knight.attack_animation_timer = 0;
            state->knight.attack_animation_frame = 0;
        }

        if (IsKeyDown(KEY_KP_6) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            state->knight.state = KNIGHT_DEFENDING;
            state->knight.state_timer = (ACTION_DURATION * attack_speed_modifier);
        }
    }
    
    // SECTION 5: Sortie de défense
    if (state->knight.state == KNIGHT_DEFENDING && (!IsKeyDown(KEY_KP_6) && !IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) && !IsButtonHovered(state->btn_defend)) {
        state->knight.state = KNIGHT_IDLE;
        state->knight.state_timer = 0;
    }
}

// Met à jour la hitbox d'attaque selon la position du joueur
void UpdateAttackHitboxesFromPlayer(CombatState *state, float playerPixelX, float playerPixelY) {
    UpdateAttackHitboxes(&state->knight, playerPixelX, playerPixelY);
}

// Récupère la hitbox d'attaque devant
Rectangle GetAttackHitboxFront(const CombatState *state) {
    return state->knight.attack_hitbox_front;
}

// Récupère la hitbox d'attaque derrière
Rectangle GetAttackHitboxBack(const CombatState *state) {
    return state->knight.attack_hitbox_back;
}

// Affiche l'UI du combat
void DrawCombat(const CombatState *state) {
    // SECTION 1: Barre de vie
    int bar_width = 300;
    int bar_height = 40;
    int bar_x = 10;
    int bar_y = 20;
    
    int hp_percent = (state->knight.hp * 100) / state->knight.max_hp;
    
    if (!editor_active) {
        DrawRectangle(bar_x, bar_y, bar_width, bar_height, RED);
        DrawRectangle(bar_x, bar_y, (bar_width * hp_percent) / 100, bar_height, GREEN);
        DrawRectangleLines(bar_x, bar_y, bar_width, bar_height, WHITE);
        
        DrawText(TextFormat("HP: %d/%d", state->knight.hp, state->knight.max_hp), 
                bar_x + 5, bar_y + 2, 24, WHITE);

        if (state->knight.state == KNIGHT_DEFENDING) {
            DrawText("DEFENSE ACTIVE", bar_x + 160, bar_y + 2, 24, BLUE);
        }
    }

    if (!state->combat_overlay_active) return;

    // SECTION 2: Instructions de contrôle
    DrawText("Attaque : Clic Gauche ou  4 (pavé numérique)  |  Défense : Clic Droit ou  6 (pavé numérique)", 10, 680, 14, YELLOW);

    // SECTION 3: Direction actuelle
    const char* direction_text = "";
    switch (state->knight.facing_direction) {
        case DIR_UP:
            direction_text = "Direction: HAUT (Z)";
            break;
        case DIR_DOWN:
            direction_text = "Direction: BAS (S)";
            break;
        case DIR_LEFT:
            direction_text = "Direction: GAUCHE (Q)";
            break;
        case DIR_RIGHT:
            direction_text = "Direction: DROITE (D)";
            break;
    }
    DrawText(direction_text, 10, 70, 14, WHITE);
}

// Active/désactive l'overlay de combat
void ToggleCombatOverlay(void) {
    gCombatState.combat_overlay_active = !gCombatState.combat_overlay_active;
}


