#ifndef _GAME_H
#define _GAME_H

// Include raylib header
#include <raylib.h>

typedef enum { 
    SCREEN_LOGO = 0, 
    SCREEN_TITLE = 1, 
    SCREEN_LEVEL = 5,
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
extern int level;
extern int highscore;
extern int score;
extern int tmpCount;
extern int ducksAdded;
extern int birdsAdded;
extern int ducksShot;
extern bool newHighscore;
extern GameScreen currentScreen;
extern unsigned int screenScale;
extern unsigned int prevScreenScale;
extern int levelDuckCount[21];
extern RenderTexture2D target;
extern Texture2D spriteSheetWater;
extern Rectangle frameRecWater[4];
extern Texture2D spriteSheetPlayer;
extern Rectangle frameRecPlayer[11];
extern Rectangle frameRecLog[4];
extern Rectangle frameRecLog[];
extern Texture spriteSheetLogs;
extern Texture2D titleTexture;
extern Texture2D raylibTexture;
extern Texture2D overlayTexture;
extern Texture2D spriteSheetDuck;
extern Texture2D toobinTexture;
extern Sprite* ripples;
extern Sprite* bullits;
extern Sprite* duckies;
extern Sprite* birds;
extern Sprite* shit;
extern Sprite* logs;
extern Sprite player;
extern Sprite* points;
extern Font konamiFont ;
extern Sound splashSfxL;
extern Sound splashSfxR;
extern Sound explodingTubeSfx;
extern Sound popperSfx[];
extern Sound seagullSfx[];
extern Sound inflateSfx;
extern Sound woodSfx;
extern Sound levelSfx;
extern Sound gameOverSfx;
extern Music music;
extern float timePlayed;
extern Font konamiFont;

extern int DEAD;
extern int ALIVE;
extern int score;
extern char levelText[50];
extern Sprite* points;
// Include raylib header


// Function declarations
extern void UpdateDrawFrame(void);
extern void UpdateGameplayScreen(void);
extern void DrawGameplayScreen(void);
extern void UpdateGameoverScreen(void);
extern void DrawGameoverScreen(void);
extern void DrawLogoScreen(void);
extern void DrawTitleScreen(void);
extern void DrawEndingScreen(void);
extern void DrawLevelScreen(void);
extern void DrawEnergyBar(Sprite player);
extern void DrawStyledText(const char* text, Vector2 position, Color textColor, Color outlineColor, int outlineThickness, int bold);


#endif // _GAME_H