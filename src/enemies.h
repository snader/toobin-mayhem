// enemies.h

#ifndef ENEMIES_H
#define ENEMIES_H

#include "raylib.h"
#include <stddef.h>

extern Texture2D spriteSheetPlayer;

// Enemy Definition
typedef struct {
    int id;
    int x;
    int y;    
    float degrees;
    int frame;
    int isAlive;
} Enemy;

// Function declaration
void DrawEnemies(Enemy enemies[], size_t size);

#endif // ENEMIES_H