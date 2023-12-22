// sprites.h

#ifndef SPRITES_H
#define SPRITES_H

#include "raylib.h"

#include <stddef.h>

extern Texture2D spriteSheetPlayer;
extern Sound popperSfx[];
extern Sound scratchSfx[];
extern int ducksAdded;
extern int score;
extern int nrOfDucks;
extern int ducksShot;
extern int level;

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
    int state;
    int secondsAlive;
    int counter;
    float energy;
} Sprite;

// Function declaration
// Draw sprites
void DrawPlayerSprite(Sprite *sprite);
void UpdatePlayerSprite(Sprite *sprite);
void DrawRipples(Sprite ripples[], size_t size);
void NewRipple(Sprite ripples[], size_t size, int x, int y);
void DrawBullits(Sprite bullits[], size_t size);
void NewBullit(Sprite bullits[], size_t size, int x, int y, int degrees);
void DrawDucks(Sprite duckies[]);
void NewDuck(Sprite duckies[]);
void UpdateDucks(Sprite duckies[], Sprite *player, Sprite bullits[]);
int calculateDistance(Vector2 v1, Vector2 v2);
int ducksAlive();
void DrawFullscreen(Texture2D overlayTexture);

#endif // SPRITES_H