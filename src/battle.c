/*Système de combat en overlay - les boutons Attaquer/Défendre s'affichent sur la map
Le joueur peut continuer à se déplacer avec les flèches et attaquer avec les boutons
Appuyer sur B pour activer/désactiver le mode combat
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "game.h"
#include "battle.h"

#define ACTION_DURATION 1.0f
#define DEFEND_REDUCTION 0.5f

double attack_power = 10;
double hitbox_width = 32;   // Largeur de la hitbox d'attaque
double hitbox_height = 32;  // Hauteur de la hitbox d'attaque

extern double attack_speed_modifier;
extern double range_modifier;

extern bool editor_active;
extern bool is_in_shop;

// Variable globale du combat
CombatState gCombatState = {0};

static bool IsButtonHovered(Rectangle btn) {
    return CheckCollisionPointRec(GetMousePosition(), btn);
}

static bool IsButtonClicked(Rectangle btn) {
    return IsButtonHovered(btn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

// Fonction pour mettre à jour les hitbox d'attaque basées sur la direction et la position du joueur
static void UpdateAttackHitboxes(Knight *knight, float playerPixelX, float playerPixelY) {
    float centerX = playerPixelX + (TILE_SIZE / 2);
    float centerY = playerPixelY + (TILE_SIZE / 2);
    
    // Hitbox devant le chevalier (rectangle couvrant 1-2 cases en diagonale)
    switch (knight->facing_direction) {
        case DIR_UP:
            // Une case devant et une à gauche et une à droite
            knight->attack_hitbox_front = (Rectangle){playerPixelX - TILE_SIZE, playerPixelY - (range_modifier * TILE_SIZE), 3*TILE_SIZE, TILE_SIZE * range_modifier};
            break;
        case DIR_DOWN:
            // Une case devant et une à gauche et une à droite
            knight->attack_hitbox_front = (Rectangle){playerPixelX - TILE_SIZE, playerPixelY + TILE_SIZE, 3*TILE_SIZE, TILE_SIZE * range_modifier};
            break;
        case DIR_LEFT:
            // Une case devant et une au-dessus et une au-dessous
            knight->attack_hitbox_front = (Rectangle){playerPixelX - (range_modifier * TILE_SIZE), playerPixelY - TILE_SIZE, TILE_SIZE * range_modifier, 3*TILE_SIZE};
            break;
        case DIR_RIGHT:
            // Une case devant et une au-dessus et une au-dessous
            knight->attack_hitbox_front = (Rectangle){playerPixelX + TILE_SIZE, playerPixelY - TILE_SIZE, TILE_SIZE * range_modifier, 3*TILE_SIZE};
            break;
    }
}

void InitCombat(CombatState *state) {
    // Initialiser le chevalier
    state->knight.hp = 100;
    state->knight.max_hp = 100;
    state->knight.attack_power = attack_power;
    state->knight.defense = 5;
    state->knight.state = KNIGHT_IDLE;
    state->knight.state_timer = 0;
    state->knight.facing_direction = DIR_DOWN;  // Direction initiale
    state->knight.attack_animation_timer = 0;   // Timer pour l'animation d'attaque
    state->knight.attack_animation_frame = 0;   // Frame courant (0-3)
    
    // Initialiser les hitbox
    state->knight.attack_hitbox_front = (Rectangle){0, 0, 0, 0};
    state->knight.attack_hitbox_back = (Rectangle){0, 0, 0, 0};

    // Initialiser les boutons (bas de l'écran)
    state->btn_attack = (Rectangle){500, 650, 140, 50};
    state->btn_defend = (Rectangle){640, 650, 140, 50};

    state->combat_overlay_active = false;
}

void UpdateCombat(CombatState *state, float dt) {
    if (!state->combat_overlay_active) return;

    // ===== MISE À JOUR DE LA DIRECTION =====
    // Sur clavier AZERTY: KEY_W=Z physique, KEY_A=Q physique, KEY_S=S, KEY_D=D
    Direction newDirection = state->knight.facing_direction;
    if (IsKeyDown(KEY_W)) {  // Touche Z sur AZERTY
        newDirection = DIR_UP;
    } else if (IsKeyDown(KEY_S)) {  // Touche S
        newDirection = DIR_DOWN;
    } else if (IsKeyDown(KEY_A)) {  // Touche Q sur AZERTY
        newDirection = DIR_LEFT;
    } else if (IsKeyDown(KEY_D)) {  // Touche D
        newDirection = DIR_RIGHT;
    }
    
    // Met à jour la direction si elle a changé
    if (newDirection != state->knight.facing_direction) {
        state->knight.facing_direction = newDirection;
    }

    // ===== CHEVALIER =====
    // Décrémente le timer
    if (state->knight.state_timer > 0) {
        state->knight.state_timer -= dt;
        if (state->knight.state_timer <= 0) {
            state->knight.state = KNIGHT_IDLE;
        }
    }
    
    // Gestion de l'animation d'attaque
    if (state->knight.state == KNIGHT_ATTACKING) {
        state->knight.attack_animation_timer += dt;
        
        // Changement de frame toutes les 0.1 secondes
        state->knight.attack_animation_frame = (int)(state->knight.attack_animation_timer / 0.1f);
        
        // Limiter à 4 frames (indices 0-3)
        if (state->knight.attack_animation_frame > 3) {
            state->knight.attack_animation_frame = 3;
            // L'animation d'attaque est terminée, revenir à IDLE
            state->knight.state = KNIGHT_IDLE;
            state->knight.attack_animation_timer = 0;
        }
    } else {
        // Réinitialiser l'animation quand on n'attaque plus
        state->knight.attack_animation_timer = 0;
        state->knight.attack_animation_frame = 0;
    }

    // Détecte les clics sur les boutons
    if (state->knight.state == KNIGHT_IDLE) {
        if (IsButtonClicked(state->btn_attack) || (IsKeyPressed(KEY_KP_4) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (!is_in_shop && !editor_active)))) {
            state->knight.state = KNIGHT_ATTACKING;
            state->knight.state_timer = (ACTION_DURATION * attack_speed_modifier);
            state->knight.attack_animation_timer = 0;  // Réinitialiser le timer d'animation
            state->knight.attack_animation_frame = 0;  // Commencer au frame 0
            TraceLog(LOG_INFO, "Chevalier attaque dans la direction %d! Hitbox front: (%f, %f, %f, %f)", 
                     state->knight.facing_direction,
                     state->knight.attack_hitbox_front.x,
                     state->knight.attack_hitbox_front.y,
                     state->knight.attack_hitbox_front.width,
                     state->knight.attack_hitbox_front.height);
        }

       
        if (IsButtonClicked(state->btn_defend) || (IsKeyDown(KEY_KP_6) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))) {
            state->knight.state = KNIGHT_DEFENDING;
            state->knight.state_timer = (ACTION_DURATION * attack_speed_modifier);
            TraceLog(LOG_INFO, "Chevalier se défend!");
        }
    }
    
    // Vérifier si la touche 6 est relâchée pour sortir de la défense
    if (state->knight.state == KNIGHT_DEFENDING && (!IsKeyDown(KEY_KP_6) && !IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) && !IsButtonHovered(state->btn_defend)) {
        state->knight.state = KNIGHT_IDLE;
        state->knight.state_timer = 0;
    }
}

// Fonction pour obtenir la position du joueur (doit être appelée avec les informations du Board)
void UpdateAttackHitboxesFromPlayer(CombatState *state, float playerPixelX, float playerPixelY) {
    UpdateAttackHitboxes(&state->knight, playerPixelX, playerPixelY);
}

// Fonction pour récupérer la hitbox d'attaque devant
Rectangle GetAttackHitboxFront(const CombatState *state) {
    return state->knight.attack_hitbox_front;
}

// Fonction pour récupérer la hitbox d'attaque derrière
Rectangle GetAttackHitboxBack(const CombatState *state) {
    return state->knight.attack_hitbox_back;
}

void DrawCombat(const CombatState *state) {

    // === BARRE DE VIE DU JOUEUR ===
    int bar_width = 150;
    int bar_height = 20;
    int bar_x = 10;
    int bar_y = 40;
    
    int hp_percent = (state->knight.hp * 100) / state->knight.max_hp;
    
    // Fond rouge (HP manquant)
    DrawRectangle(bar_x, bar_y, bar_width, bar_height, RED);
    // HP restant en vert
    DrawRectangle(bar_x, bar_y, (bar_width * hp_percent) / 100, bar_height, GREEN);
    // Bordure blanche
    DrawRectangleLines(bar_x, bar_y, bar_width, bar_height, WHITE);
    
    // Texte HP
    DrawText(TextFormat("HP: %d/%d", state->knight.hp, state->knight.max_hp), 
             bar_x + 5, bar_y + 2, 12, WHITE);
    
    if (!state->combat_overlay_active) return;
    
    // État de défense
    if (state->knight.state == KNIGHT_DEFENDING) {
        DrawText("DEFENSE ACTIVE", bar_x + 160, bar_y + 2, 12, BLUE);
    }

    // === BOUTONS (overlay sur la map) ===
    Color btn_attack_color = IsButtonHovered(state->btn_attack) ? GRAY : RED;
    Color btn_defend_color = IsButtonHovered(state->btn_defend) ? GRAY : BLUE;

    // Boutons désactivés si le chevalier n'est pas en IDLE
    if (state->knight.state != KNIGHT_IDLE) {
        btn_attack_color = GRAY;
        btn_defend_color = GRAY;
    }

    DrawRectangleRec(state->btn_attack, btn_attack_color);
    DrawRectangleLinesEx(state->btn_attack, 2, WHITE);
    DrawText("ATTAQUER", (int)state->btn_attack.x + 25, (int)state->btn_attack.y + 15, 14, WHITE);

    DrawRectangleRec(state->btn_defend, btn_defend_color);
    DrawRectangleLinesEx(state->btn_defend, 2, WHITE);
    DrawText("DEFENDRE", (int)state->btn_defend.x + 22, (int)state->btn_defend.y + 15, 14, WHITE);

    // Affiche les hitboxes d'attaque en carré rouge
    DrawRectangleRec(state->knight.attack_hitbox_front, (Color){255, 0, 0, 100});  // Rouge semi-transparent
    DrawRectangleLinesEx(state->knight.attack_hitbox_front, 2, RED);  // Bordure rouge
    
    DrawRectangleRec(state->knight.attack_hitbox_back, (Color){255, 0, 0, 50});   // Rouge très transparent
    DrawRectangleLinesEx(state->knight.attack_hitbox_back, 1, (Color){255, 0, 0, 150});

    // Affiche la direction actuelle du chevalier
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

void ToggleCombatOverlay(void) {
    gCombatState.combat_overlay_active = !gCombatState.combat_overlay_active;
    if (gCombatState.combat_overlay_active) {
        TraceLog(LOG_INFO, "Mode combat activé!");
    } else {
        TraceLog(LOG_INFO, "Mode combat désactivé!");
    }
}


