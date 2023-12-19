#ifndef _GAME_H
#define _GAME_H


typedef enum { 
    SCREEN_LOGO = 0, 
    SCREEN_TITLE = 1, 
    SCREEN_GAMEPLAY = 2, 
    SCREEN_GAMEOVER = 3, 
    SCREEN_ENDING = 4
} GameScreen;

// ... Existing declarations ...

// Global variables
extern int screenWidth;
extern int screenHeight;
extern int frameCount;
extern int waterFrame;
extern GameScreen currentScreen;
extern unsigned int screenScale;
extern unsigned int prevScreenScale;

extern RenderTexture2D target;
extern Texture2D spriteSheetWater;
extern Rectangle frameRecWater[4];
extern Texture2D spriteSheetPlayer;
extern Rectangle frameRecPlayer[11];
extern Sprite* ripples;
extern Sprite* bullits;
extern Sprite* duckies;
extern Sprite player;

extern Sound splashSfxL;
extern Sound splashSfxR;
extern Sound explodingTubeSfx;
extern Sound popperSfx[];

extern int DEAD;
extern int ALIVE;

// Include raylib header
#include <raylib.h>

// Function declarations
extern void UpdateDrawFrame(void);
extern void UpdateGameplayScreen(void);
extern void DrawGameplayScreen(void);
extern void UpdateGameoverScreen(void);
extern void DrawGameoverScreen(void);
extern void DrawLogoScreen(void);
extern void DrawTitleScreen(void);
extern void DrawEndingScreen(void);

#endif // _GAME_H