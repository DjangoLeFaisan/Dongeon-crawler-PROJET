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
#define ATTACK_POWER 15
#define DEFEND_REDUCTION 0.5f

// Variable globale du combat
CombatState gCombatState = {0};

static bool IsButtonHovered(Rectangle btn) {
    return CheckCollisionPointRec(GetMousePosition(), btn);
}

static bool IsButtonClicked(Rectangle btn) {
    return IsButtonHovered(btn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void InitCombat(CombatState *state) {
    // Initialiser le chevalier
    state->knight.hp = 100;
    state->knight.max_hp = 100;
    state->knight.attack_power = ATTACK_POWER;
    state->knight.defense = 5;
    state->knight.state = KNIGHT_IDLE;
    state->knight.state_timer = 0;

    // Initialiser les boutons (bas de l'écran)
    state->btn_attack = (Rectangle){150, 650, 140, 50};
    state->btn_defend = (Rectangle){350, 650, 140, 50};

    state->combat_overlay_active = false;
}

void UpdateCombat(CombatState *state, float dt) {
    if (!state->combat_overlay_active) return;

    // ===== CHEVALIER =====
    // Décrémente le timer
    if (state->knight.state_timer > 0) {
        state->knight.state_timer -= dt;
        if (state->knight.state_timer <= 0) {
            state->knight.state = KNIGHT_IDLE;
        }
    }

    // Détecte les clics sur les boutons
    if (IsButtonClicked(state->btn_attack) && state->knight.state == KNIGHT_IDLE) {
        state->knight.state = KNIGHT_ATTACKING;
        state->knight.state_timer = ACTION_DURATION;
        TraceLog(LOG_INFO, "Chevalier attaque!");
    }

    if (IsButtonClicked(state->btn_defend) && state->knight.state == KNIGHT_IDLE) {
        state->knight.state = KNIGHT_DEFENDING;
        state->knight.state_timer = ACTION_DURATION;
        TraceLog(LOG_INFO, "Chevalier se défend!");
    }
}

void DrawCombat(const CombatState *state) {
    if (!state->combat_overlay_active) return;

    // === BOUTONS (overlay sur la map) ===
    Color btn_attack_color = IsButtonHovered(state->btn_attack) ? ORANGE : DARKGRAY;
    Color btn_defend_color = IsButtonHovered(state->btn_defend) ? SKYBLUE : DARKGRAY;

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

    // Affiche le timer d'action
    if (state->knight.state_timer > 0) {
        DrawText(TextFormat("Cooldown: %.1fs", state->knight.state_timer),
                 150, 590, 14, YELLOW);
    }

    // Affiche l'état du chevalier
    const char* knight_state_text = (state->knight.state == KNIGHT_ATTACKING) ? "En attaque!" :
                                    (state->knight.state == KNIGHT_DEFENDING) ? "En défense!" : "";
    if (knight_state_text[0] != '\0') {
        DrawText(knight_state_text, 150, 615, 12, YELLOW);
    }

}

void ToggleCombatOverlay(void) {
    gCombatState.combat_overlay_active = !gCombatState.combat_overlay_active;
    if (gCombatState.combat_overlay_active) {
        TraceLog(LOG_INFO, "Mode combat activé!");
    } else {
        TraceLog(LOG_INFO, "Mode combat désactivé!");
    }
}

int choisirQuiCommence(void) {
    int resultatordi = rand() % 2;
    if (resultatordi == 1) {
        TraceLog(LOG_INFO, "L'ennemi commence!");
    } else {
        TraceLog(LOG_INFO, "Le chevalier commence!");
    }
    return resultatordi;
}
