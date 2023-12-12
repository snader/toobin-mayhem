// sprites.h

#ifndef SPRITES_H
#define SPRITES_H

#include "raylib.h"
#include <stddef.h>

extern Texture2D spriteSheetPlayer;

// Sprite Definition
typedef struct {
    int id;
    float x;
    float y;    
    float degrees;
    float speed;
    int frame;
    int frameDelay;
    int isAlive;
    int drawFrame;
} Sprite;

// Function declaration
// Draw enemies
void DrawPlayerSprite(Sprite *sprite);
void UpdatePlayerSprite(Sprite *sprite);


#endif // ENEMIES_H