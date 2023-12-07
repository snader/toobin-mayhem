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
#include "enemies.h"

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 
#include <stddef.h>

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

// GameScreen Definition
typedef enum { 
    SCREEN_LOGO = 0, 
    SCREEN_TITLE = 1, 
    SCREEN_GAMEPLAY = 2, 
    SCREEN_ENDING = 3
} GameScreen;


// TODO: Define your custom data types here



//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static const int screenWidth = 256;
static const int screenHeight = 256;

static unsigned int screenScale = 3; 
static unsigned int prevScreenScale = 1;

static int frameCount = 0;

static RenderTexture2D target = { 0 };  // Initialized at init

static GameScreen currentScreen = SCREEN_LOGO;

// 
static const int DEAD = 0;
static const int ALIVE = 1;

// Player sprite
Texture2D spriteSheetPlayer;
int frameWidthPlayer = 26;
int frameHeightPlayer = 25;
static int frameCountPlayer = 11;
static Rectangle frameRecPlayer[11];  // Assuming 11 frames

// Water tiles
static Texture2D spriteSheetWater;
static Rectangle frameRecWater[4]; 
static int waterFrame = 0;


// Array of enemies
Enemy* enemies = NULL;  // Declare a pointer to Enemy

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame
static void UpdateGameplayScreen(void);
static void UpdateLogoScreen(void);
static void UpdateTitleScreen(void);
static void UpdateEndingScreen(void);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif

   // GameScreen currentScreen = SCREEN_GAMEPLAY;
  
    // Initialize enemies array
    enemies = (Enemy*)malloc(50 * sizeof(Enemy));  // Assuming initial size is 50
    enemies[0] = (Enemy){1, 10, 20, 30.0f, 0, ALIVE};
    enemies[1] = (Enemy){2, 30, 40, 60.0f, 0, ALIVE};
    enemies[2] = (Enemy){3, 50, 60, 90.0f, 0, ALIVE};
    enemies[3] = (Enemy){4, 70, 80, 120.0f, 0, ALIVE}; 

      
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Toobin' Mayhem");
    
    // Load resources / Initialize variables at this point
    spriteSheetPlayer = LoadTexture("resources/player.png"); 
    for (int i = 0; i < frameCountPlayer; i++) {
        frameRecPlayer[i].x = i * frameWidthPlayer;  // Adjust based on your sprite sheet layout
        frameRecPlayer[i].y = 0;
        frameRecPlayer[i].width = frameWidthPlayer;
        frameRecPlayer[i].height = frameHeightPlayer;
    }
    spriteSheetWater = LoadTexture("resources/water.png"); 
    for (int i = 0; i < 4; i++) {
        frameRecWater[i].x = i * 16;  // Adjust based on your sprite sheet layout
        frameRecWater[i].y = 0;
        frameRecWater[i].width = 16;
        frameRecWater[i].height = 16;
    }  
    
    // Render texture to draw full screen, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    target = LoadRenderTexture(screenWidth, screenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR); // 

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
    
    

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------  
// Update and draw frame
//----------------------------------------------------------------------------------  
void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    
    // main frameCount
    frameCount++;
    if (frameCount>60) {
        frameCount = 1;
    }
        
    // Screen scale logic (x2)
    if (IsKeyPressed(KEY_ONE)) screenScale = 1;
    else if (IsKeyPressed(KEY_TWO)) screenScale = 2;
    else if (IsKeyPressed(KEY_THREE)) screenScale = 3;   
    if (screenScale != prevScreenScale)
    {
        SetWindowSize(screenWidth*screenScale, screenHeight*screenScale);                       
        prevScreenScale = screenScale;
    }
    
    // KEYS PRESSED
    if (IsKeyPressed(KEY_SPACE)){
        switch (currentScreen) {
            case SCREEN_LOGO:
                currentScreen = SCREEN_TITLE;
                break;
            case SCREEN_TITLE:
                currentScreen = SCREEN_GAMEPLAY;   
                break;
            case SCREEN_GAMEPLAY:
                
                break;
            case SCREEN_ENDING:
                
                break;
            default:
                currentScreen = SCREEN_TITLE;   
                break;
        }
    }
    
    

    // Update variables / Implement example logic at this point 
    if (frameCount % 30 == 0) {
        waterFrame = GetRandomValue(0, 3);
    }

    
    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------
    // Render all screen to texture (for scaling)
    BeginTextureMode(target);
        ClearBackground(RAYWHITE);
        
        
        switch (currentScreen) {
            case SCREEN_LOGO:
                UpdateLogoScreen();
                break;
            case SCREEN_TITLE:
                UpdateTitleScreen();
                break;
            case SCREEN_GAMEPLAY:
                UpdateGameplayScreen();
                break;
            case SCREEN_ENDING:
                UpdateEndingScreen();
                break;
            default:
                // Handle invalid screen or default case
                break;
        }
              
        
    EndTextureMode();
    
    BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw render texture to screen scaled as required
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)target.texture.width*screenScale, (float)target.texture.height*screenScale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
        
        // Scanlines
        for (int y = 0; y < target.texture.width*screenScale; y += 4) {
            DrawRectangle(0, y, (float)target.texture.width*screenScale, 1, (Color){0, 0, 0, 25}); // Adjust the alpha value for intensity
        }

        // TODO: Draw everything that requires to be drawn at this point:
        

    EndDrawing();
    //----------------------------------------------------------------------------------  
    
    
        
    
}

// *************
// Functions ***
// *************

void UpdateGameplayScreen(void) {
    // Update logic for the gameplay screen...
    //DrawText(TextFormat("In game: %d", SCREEN_GAMEPLAY), 10, 10, 20, WHITE);
    
    
    // Draw water
        for (int y = 0; y < 256; y=y+16) {
            for (int x = 0; x < 256; x=x+16) {
                DrawTextureRec(spriteSheetWater, frameRecWater[waterFrame], (Vector2) { x, y }, WHITE);
            }
        }
        
        // Draw the current player frame
        DrawTextureRec(spriteSheetPlayer, frameRecPlayer[0], (Vector2) { 120, 150 }, WHITE);      
        
        // DrawEnemies
        DrawEnemies(enemies, sizeof(enemies));
        
        DrawText("Game", 10, 10, 20, GREEN);
}


// Define update functions for each screen
void UpdateLogoScreen(void) {
    // Update logic for the logo screen...
    DrawText("Logo", 10, 10, 20, RED);
}

void UpdateTitleScreen(void) {
    // Update logic for the title screen...
    DrawText("Title", 10, 10, 20, RED);
}

void UpdateEndingScreen(void) {
    // Update logic for the ending screen...
}