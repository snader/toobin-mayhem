/*******************************************************************************************
*
*   raylib 9years gamejam - Toobin' Mayhem
*
*   Based on Template originally created with raylib 4.5-dev, last time updated with raylib 4.5-dev
*
*   Template licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2023 Landgoed Voorn
*
********************************************************************************************/

#include "raylib.h"
#include "sprites.h"
#include "game.h"

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 
#include <stddef.h>
#include <math.h>

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif


//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int screenWidth = 256;
int screenHeight = 256;

unsigned int screenScale = 3; 
unsigned int prevScreenScale = 1;

int frameCount = 0;
int myCount=0;
int nrOfDucks = 100;
int nrOfBirds = 5;
int ducksShot = 0;

RenderTexture2D target = { 0 };  // Initialized at init

// NOTE! replace SCREEN_GAMEPLAY before release
GameScreen currentScreen = SCREEN_LOGO; // SCREEN_LOGO;

// isAlive
int DEAD = 0;
int ALIVE = 1;

// state
int DEFAULT = 1;
int SHOOTING = 2;
int EXPLODING = 3;

// damage
int duckHit = 40;

int level = 1;
int levelDuckCount[21] = {0, 5, 8, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95};
int ducksAdded = 0;
int birdsAdded = 0;

int score = 0;
int highscore = 0;
bool newHighscore = false;
char formattedScore[5];

// Player sprite
Texture2D spriteSheetPlayer;
int frameWidthPlayer = 26;
int frameHeightPlayer = 25;
int frameCountPlayer = 11;
Rectangle frameRecPlayer[11];  // Assuming 11 frames

// Water tiles
Texture2D spriteSheetWater;
Rectangle frameRecWater[4]; 

// counter
Texture2D spriteCounter;
Rectangle frameRecCounter[5];

// Rubber duckies
Texture2D spriteSheetDuck;
Rectangle frameRecDuck[4];

// birds
Texture2D spriteSheetBirdWhite;
Rectangle frameRecBirdWhite[4];
Texture2D spriteSheetBirdBlue;
Rectangle frameRecBirdBlue[4];

// log
Texture2D spriteSheetLog;
Rectangle frameRecLog[4];

// titelscreen
Texture2D titleTexture; 
// explosion
Texture2D explosionTexture;
Texture2D toobinTexture;

// overlay
Texture2D overlayTexture; 

Texture2D raylibTexture;

int waterFrame = 0;
int popperSoundNr = 0;

// ripples
Texture2D spriteSheetRipple;
Rectangle frameRecRipple[4]; 
// Array of enemies
Sprite* ripples = NULL;  // Declare a pointer to sEnemy
Sprite* bullits = NULL;

Sound splashSfxL;
Sound splashSfxR;
Sound popperSfx[4];
Sound scratchSfx[3];
Sound quackSfx[4];
Sound seagullSfx[4];
Sound ouchSfx[3];
Sound fartSfx[4];
Sound explodingTubeSfx;
Sound popSfx;
Sound ewwSfx;
Sound woodSfx;
Sound inflateSfx;
Sound levelSfx;
Sound gameOverSfx;

Music music;

Font konamiFont;
int tmpCount = 0;
char levelText[50];
float timePlayed = 0.0f;        // Time played normalized [0.0f..1.0f]

// Array of enemies
Sprite* duckies = NULL;  // Declare a pointer to Sprite
Sprite* birds = NULL;
Sprite* shit = NULL;
Sprite* logs = NULL;
Sprite player; 

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void DrawEnergyBar(Sprite player) {
    // Calculate bar dimensions based on energy level
    int x = 75;
    int y = 6;
    int width = 70;
    int height = 10;
    float barWidth = width * (player.energy / 100.0);

    // Draw the bar
    if (player.energy>50) {
        DrawRectangle(x, y, barWidth, height, GREEN);
    } else if (player.energy>20) {
        DrawRectangle(x, y, barWidth, height, ORANGE);
    } else {
        DrawRectangle(x, y, barWidth, height, RED);
    }   
        
    // Draw white border
    DrawRectangleLinesEx((Rectangle){x, y, width, height}, 1, WHITE);
    
    // energy
    DrawStyledText("TUBE AIR", (Vector2){10, y}, WHITE, BLACK, 1, 1);   
    
    // duckies
    Rectangle sourceRec = {0, 0, 10, 10}; // 
    Vector2 origin = {0,0}; // Set the origin to the center of the sprite     
    DrawTexturePro(
                spriteSheetDuck,
                sourceRec,
                (Rectangle){81, y+12, 10, 10},
                origin,
                0,
                BLACK
            );  
            DrawTexturePro(
                spriteSheetDuck,
                sourceRec,
                (Rectangle){80, y+11, 10, 10},
                origin,
                0,
                WHITE
            );    
    sprintf(levelText, "SHOT %d/%d", ducksShot, levelDuckCount[level]);
    DrawStyledText(levelText, (Vector2){95, y+12}, WHITE, BLACK, 1, 1);   
    
    int textWidth = 0;
    sprintf(levelText, "HIGH %04d", highscore);
    textWidth = MeasureText(levelText, konamiFont.baseSize);
    DrawStyledText(levelText, (Vector2){screenWidth - textWidth - 20, y}, WHITE, BLACK, 1 ,1);  
    
    sprintf(levelText, "SCORE %04d", score);
    textWidth = MeasureText(levelText, konamiFont.baseSize);
    DrawStyledText(levelText, (Vector2){screenWidth - textWidth - 21, y+12}, WHITE, BLACK, 1 ,1);  
    
    sprintf(levelText, "LEVEL %02d", level);
    DrawStyledText(levelText, (Vector2){10, y+12}, WHITE, BLACK, 1 ,1);  
}


float GetRandomFloat(float min, float max) {
    // Genereer een willekeurige waarde tussen 0 en RAND_MAX
    int randomInt = GetRandomValue(0, RAND_MAX);

    // Schaal de waarde naar het gewenste bereik
    float randomFloat = min + ((float)randomInt / RAND_MAX) * (max - min);

    return randomFloat;
}


void DrawStyledText(const char* text, Vector2 position, Color textColor, Color outlineColor, int outlineThickness, int bold) {
    // Draw text with outline
    if (outlineThickness>0) {
        DrawTextEx(konamiFont, text, (Vector2){position.x + outlineThickness, position.y + outlineThickness}, konamiFont.baseSize, 2.0f, outlineColor);
        DrawTextEx(konamiFont, text, (Vector2){position.x + outlineThickness + 1, position.y + outlineThickness}, konamiFont.baseSize, 2.0f, outlineColor);
    }
    // Draw text without outline    
    DrawTextEx(konamiFont, text, position, konamiFont.baseSize, 2.0f, textColor);
    if (bold) {
        DrawTextEx(konamiFont, text, (Vector2){position.x + 1, position.y}, konamiFont.baseSize, 2.0f, textColor);
    }
}

int calculateDistance(Vector2 point1, Vector2 point2) {
    return sqrt(pow(point2.x - point1.x, 2) + pow(point2.y - point1.y, 2));
}



//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif

InitAudioDevice(); 
    
    // soundfx    
    splashSfxL = LoadSound("resources/splashLow.wav");
    splashSfxR = LoadSound("resources/splashHigh.wav");
    popperSfx[1] = LoadSound("resources/popper1.wav");
    popperSfx[2] = LoadSound("resources/popper2.wav");
    popperSfx[3] = LoadSound("resources/popper3.wav");
    quackSfx[1] = LoadSound("resources/quack1.wav");
    quackSfx[2] = LoadSound("resources/quack2.wav");
    quackSfx[3] = LoadSound("resources/quack3.wav");
    seagullSfx[1] = LoadSound("resources/seagull1.wav");
    seagullSfx[2] = LoadSound("resources/seagull2.wav");
    seagullSfx[3] = LoadSound("resources/seagull3.wav");
    levelSfx = LoadSound("resources/level.wav");
    gameOverSfx = LoadSound("resources/gameover.wav");
    
    fartSfx[1] = LoadSound("resources/fart1.wav");
    fartSfx[2] = LoadSound("resources/fart2.wav");
    fartSfx[3] = LoadSound("resources/fart3.wav");
    
    ouchSfx[1] = LoadSound("resources/ouch1.wav");
    ouchSfx[2] = LoadSound("resources/ouch2.wav");
    popSfx = LoadSound("resources/pop.wav");
    ewwSfx = LoadSound("resources/eww.wav");
    woodSfx = LoadSound("resources/wood.wav");
    //scratchSfx[1] = LoadSound("resources/scratch1.wav");
    scratchSfx[2] = LoadSound("resources/scratch2.wav");
    inflateSfx = LoadSound("resources/inflate.wav");
    
    explodingTubeSfx = LoadSound("resources/explodingtube.wav");
    //reloadSfx = LoadSound("resources/reload.wav");

    
    // Initialize enemies array
    duckies = (Sprite*)malloc(100 * sizeof(Sprite));  // Assuming initial size is 50
    birds = (Sprite*)malloc(6 * sizeof(Sprite));
    shit = (Sprite*)malloc(6 * sizeof(Sprite));
    logs = (Sprite*)malloc(8 * sizeof(Sprite));
    ripples = (Sprite*)malloc(10 * sizeof(Sprite));
    bullits = (Sprite*)malloc(50 * sizeof(Sprite));
    
    for (int i = 0; i < 49; i++) {
        bullits[i] = (Sprite){1, 100, 100, 0.0f, 0.0f, 0, 0, DEAD};
    }
    
    for (int i = 1; i < nrOfDucks; i++) {
        duckies[i] = (Sprite){1, 100, 100, 0.0f, 0.0f, 0, 0, DEAD};       
    }
    for (int i = 1; i < nrOfBirds; i++) {
        birds[i] = (Sprite){1, 100, 100, 0.0f, 0.0f, 0, 0, DEAD};     
    }
    for (int i = 1; i < nrOfBirds; i++) {
        shit[i] = (Sprite){1, 100, 100, 0.0f, 0.0f, 0, 0, DEAD};     
    }
    for (int i = 1; i < 8; i++) {
        logs[i] = (Sprite){1, 100, 100, 0.0f, 0.0f, 0, 0, DEAD};     
    }
   
    player = (Sprite){1, 100, 100, 0.0f, 0.0f, 0, 0, DEAD};
 
    ripples[0] = (Sprite){1, 100, 100, 0.0f, 0.0f, 0, 0, DEAD};
    ripples[1] = (Sprite){1, 130, 100, 0.0f, 0.0f, 0, 0, DEAD};
    ripples[2] = (Sprite){1, 150, 100, 0.0f, 0.0f, 0, 0, DEAD};
    ripples[3] = (Sprite){1, 160, 100, 0.0f, 0.0f, 0, 0, DEAD};
    ripples[4] = (Sprite){1, 180, 130, 0.0f, 0.0f, 0, 0, DEAD};
      
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Toobin' Mayhem");
    
    // Load resources / Initialize variables at this point
    konamiFont = LoadFont("resources/font.png");
    
    titleTexture = LoadTexture("resources/title.png");
    toobinTexture = LoadTexture("resources/toobin.png");
    overlayTexture = LoadTexture("resources/boardwalk.png");
    raylibTexture = LoadTexture("resources/raylib.png");
    explosionTexture = LoadTexture("resources/explosion.png");
    
     // Load MP3 audio file
    music = LoadMusicStream("resources/lake.mp3");
    
    spriteSheetPlayer = LoadTexture("resources/player2.png"); 
    for (int i = 0; i < frameCountPlayer; i++) {
        frameRecPlayer[i].x = i * frameWidthPlayer;  // Adjust based on your sprite sheet layout
        frameRecPlayer[i].y = 0;
        frameRecPlayer[i].width = frameWidthPlayer;
        frameRecPlayer[i].height = frameHeightPlayer;
    }
    spriteSheetWater = LoadTexture("resources/water2.png"); 
    for (int i = 0; i < 4; i++) {
        frameRecWater[i].x = i * 16;  // Adjust based on your sprite sheet layout
        frameRecWater[i].y = 0;
        frameRecWater[i].width = 16;
        frameRecWater[i].height = 16;
    }  
    spriteSheetRipple = LoadTexture("resources/ripple.png");
    for (int i = 0; i < 4; i++) {
        frameRecRipple[i].x = i * 11;  // Adjust based on your sprite sheet layout
        frameRecRipple[i].y = 0;
        frameRecRipple[i].width = 1;
        frameRecRipple[i].height = 1;
    }  
    spriteSheetDuck = LoadTexture("resources/rubberduck-small.png"); 
    for (int i = 0; i < 4; i++) {
        frameRecDuck[i].x = i * 10;  // Adjust based on your sprite sheet layout
        frameRecDuck[i].y = 0;
        frameRecDuck[i].width = 10;
        frameRecDuck[i].height = 10;
    }
    spriteSheetBirdWhite = LoadTexture("resources/bird2.png"); 
    for (int i = 0; i < 4; i++) {
        frameRecBirdWhite[i].x = i * 18;  // Adjust based on your sprite sheet layout
        frameRecBirdWhite[i].y = 0;
        frameRecBirdWhite[i].width = 18;
        frameRecBirdWhite[i].height = 16;
    }  
    spriteSheetBirdBlue = LoadTexture("resources/bird1.png"); 
    for (int i = 0; i < 4; i++) {
        frameRecBirdBlue[i].x = i * 18;  // Adjust based on your sprite sheet layout
        frameRecBirdBlue[i].y = 0;
        frameRecBirdBlue[i].width = 18;
        frameRecBirdBlue[i].height = 16;
    }  
    spriteCounter = LoadTexture("resources/count.png"); 
    for (int i = 0; i < 4; i++) {
        frameRecCounter[i].x = i * 5;  // Adjust based on your sprite sheet layout
        frameRecCounter[i].y = 0;
        frameRecCounter[i].width = 5;
        frameRecCounter[i].height = 5;
    }  
    spriteSheetLog = LoadTexture("resources/log.png"); 
    frameRecLog[0].x = 0;  // big log
    frameRecLog[0].y = 0;
    frameRecLog[0].width = 28;
    frameRecLog[0].height = 19;
    frameRecLog[1].x = 30;  // small log 1
    frameRecLog[1].y = 5;
    frameRecLog[1].width = 12;
    frameRecLog[1].height = 8;
    frameRecLog[2].x = 44;  // small log 2
    frameRecLog[2].y = 6;
    frameRecLog[2].width = 9;
    frameRecLog[2].height = 7;
   
    
    // Render texture to draw full screen, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR); //  TEXTURE_FILTER_BILINEAR
    
    // Play the music
    PlayMusicStream(music);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    

    

    SetTargetFPS(60);     // Set our game frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button
    {       
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);
    
    // TODO: Unload all loaded resources at this point
    UnloadTexture(spriteSheetPlayer);
    UnloadTexture(spriteSheetWater);
    UnloadTexture(spriteSheetRipple);
    UnloadTexture(spriteSheetDuck);
    UnloadTexture(spriteSheetLog);
    UnloadTexture(spriteSheetBirdWhite);
    UnloadTexture(spriteSheetBirdBlue);
    UnloadTexture(explosionTexture);
    UnloadTexture(titleTexture);
    UnloadTexture(overlayTexture);
    UnloadMusicStream(music);
    
    UnloadSound(splashSfxR);
    UnloadSound(splashSfxL);
    UnloadSound(popperSfx[1]);
    UnloadSound(popperSfx[2]);
    UnloadSound(popperSfx[3]);
    UnloadSound(quackSfx[1]);
    UnloadSound(quackSfx[2]);
    UnloadSound(quackSfx[3]);
    UnloadSound(seagullSfx[1]);
    UnloadSound(seagullSfx[2]);
    UnloadSound(seagullSfx[3]);
    UnloadSound(fartSfx[1]);
    UnloadSound(fartSfx[2]);
    UnloadSound(fartSfx[3]);
    UnloadSound(levelSfx);
    UnloadSound(gameOverSfx);
    UnloadSound(scratchSfx[2]);
    UnloadSound(ewwSfx);
    
    UnloadSound(explodingTubeSfx);
    UnloadSound(ouchSfx[1]);
    UnloadSound(ouchSfx[2]);
    UnloadSound(popSfx);
    UnloadSound(inflateSfx);
    UnloadSound(woodSfx);
    
    free(ripples);
    free(bullits);
    free(duckies);
    free(birds);
    free(shit);
    free(logs);
       
    CloseAudioDevice();   

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------
