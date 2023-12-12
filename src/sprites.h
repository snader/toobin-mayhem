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
    int isAlive;
} Sprite;

// Function declaration
// Draw enemies
void DrawSprites(Sprite enemies[], size_t size);
void UpdateSprite(Sprite *sprite);


#endif // ENEMIES_H