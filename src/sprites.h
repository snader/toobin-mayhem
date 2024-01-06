// sprites.h

#ifndef SPRITES_H
#define SPRITES_H

#include "raylib.h"

#include <stddef.h>

extern Texture2D spriteSheetPlayer;
extern Sound popperSfx[];
extern Sound scratchSfx[];
extern Sound woodSfx;
extern int ducksAdded;
extern int birdsAdded;
extern int highscore;
extern int score;
extern int nrOfDucks;
extern int nrOfBirds;
extern int ducksShot;
extern int level;
extern Font konamiFont;

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
void NewScores(Sprite points[], int x, int y, int score);
void UpdateDucks(Sprite duckies[], Sprite *player, Sprite bullits[], Sprite logs[], Sprite points[]);
float GetRandomFloat(float min, float max);
void DrawBirds(Sprite birds[]);
void DrawShit(Sprite shitp[]);
void DrawLogs(Sprite logs[]);
void DrawScores(Sprite points[]);
void NewBird(Sprite birds[], Sprite *player);
void NewShit(Sprite shit[], Sprite birds[], Sprite *player);
void NewLog(Sprite logs[], int x, int y, int size, float speed, float degrees);
void UpdateBirds(Sprite birds[], Sprite *player);
void UpdateShit(Sprite shit[], Sprite *player);
void UpdateLogs(Sprite logs[], Sprite *player, Sprite bullits[], Sprite points[]);
int calculateDistance(Vector2 v1, Vector2 v2);
int ducksAlive();
void DrawFullscreen(Texture2D overlayTexture);

#endif // SPRITES_H