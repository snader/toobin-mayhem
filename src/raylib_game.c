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

Texture2D titleTexture; 

int waterFrame = 0;
int popperSoundNr = 0;

// ripples
Texture2D spriteSheetRipple;
Rectangle frameRecRipple[4]; 
// Array of enemies
Sprite* ripples = NULL;  // Declare a pointer to Enemy
Sprite* bullits = NULL;

Sound splashSfxL;
Sound splashSfxR;
Sound popperSfx[4];
Sound quackSfx[4];
Sound ouchSfx[3];
Sound explodingTubeSfx;
Sound popSfx;


// Array of enemies
Sprite* duckies = NULL;  // Declare a pointer to Sprite
Sprite player; 

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void DrawEnergyBar(Sprite player) {
    // Calculate bar dimensions based on energy level
    float barWidth = (80.0) * player.energy / 100.0;

    // Draw the bar
    if (player.energy>50) {
        DrawRectangle(10, 10, barWidth, 10, GREEN);
    } else if (player.energy>20) {
        DrawRectangle(10, 10, barWidth, 10, ORANGE);
    } else {
        DrawRectangle(10, 10, barWidth, 10, RED);
    }   
        
    // Draw white border
    DrawRectangleLinesEx((Rectangle){10, 10, 80, 10}, 1, WHITE);
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
    ouchSfx[1] = LoadSound("resources/ouch1.wav");
    ouchSfx[2] = LoadSound("resources/ouch2.wav");
    popSfx = LoadSound("resources/pop.wav");
    
    explodingTubeSfx = LoadSound("resources/explodingtube.wav");
    //reloadSfx = LoadSound("resources/reload.wav");

    
    // Initialize enemies array
    duckies = (Sprite*)malloc(20 * sizeof(Sprite));  // Assuming initial size is 50
    ripples = (Sprite*)malloc(10 * sizeof(Sprite));
    bullits = (Sprite*)malloc(50 * sizeof(Sprite));
    
    for (int i = 0; i < 49; i++) {
        bullits[i] = (Sprite){1, 100, 100, 0.0f, 0.0f, 0, 0, DEAD};
    }
    
    for (int i = 0; i < 20; i++) {
        duckies[i] = (Sprite){1, 100, 100, 0.0f, 0.0f, 0, 0, DEAD};
    }
    
    //enemies[1] = (Sprite){2, 30, 40, 60.0f, 0, ALIVE};
    //enemies[2] = (Sprite){3, 50, 60, 90.0f, 0, ALIVE};
    //enemies[3] = (Sprite){4, 70, 80, 120.0f, 0, ALIVE}; 
    
    player = (Sprite){1, 100, 100, 0.0f, 0.0f, 0, 0, ALIVE};
 
    ripples[0] = (Sprite){1, 100, 100, 0.0f, 0.0f, 0, 0, DEAD};
    ripples[1] = (Sprite){1, 130, 100, 0.0f, 0.0f, 0, 0, DEAD};
    ripples[2] = (Sprite){1, 150, 100, 0.0f, 0.0f, 0, 0, DEAD};
    ripples[3] = (Sprite){1, 160, 100, 0.0f, 0.0f, 0, 0, DEAD};
    ripples[4] = (Sprite){1, 180, 130, 0.0f, 0.0f, 0, 0, DEAD};
      
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Toobin' Mayhem");
    
    // Load resources / Initialize variables at this point
    
    titleTexture = LoadTexture("resources/title.png");
    
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
    spriteCounter = LoadTexture("resources/count.png"); 
    for (int i = 0; i < 4; i++) {
        frameRecCounter[i].x = i * 5;  // Adjust based on your sprite sheet layout
        frameRecCounter[i].y = 0;
        frameRecCounter[i].width = 5;
        frameRecCounter[i].height = 5;
    }  
    
    // Render texture to draw full screen, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR); //  TEXTURE_FILTER_BILINEAR
    

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
    
    UnloadSound(splashSfxR);
    UnloadSound(splashSfxL);
    UnloadSound(popperSfx[1]);
    UnloadSound(popperSfx[2]);
    UnloadSound(popperSfx[3]);
    UnloadSound(quackSfx[1]);
    UnloadSound(quackSfx[2]);
    UnloadSound(quackSfx[3]);
    
    UnloadSound(explodingTubeSfx);
    UnloadSound(ouchSfx[1]);
    UnloadSound(ouchSfx[2]);
    
    free(ripples);
    free(bullits);
    free(duckies);
       
    CloseAudioDevice();   

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------
