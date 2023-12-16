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
// Draw sprites
void DrawPlayerSprite(Sprite *sprite);
void UpdatePlayerSprite(Sprite *sprite);
void DrawRipples(Sprite ripples[], size_t size);
void NewRipple(Sprite ripples[], size_t size, int x, int y);



#endif // $PRITES_H