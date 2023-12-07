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

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 
#include <time.h>

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
    SCREEN_TITLE, 
    SCREEN_GAMEPLAY, 
    SCREEN_ENDING
} GameScreen;

// Enemy Definition
typedef struct {
    int id;
    int x;
    int y;    
    float degrees;
    int frame;
    int isAlive;
} Enemy;

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

// 
static const int DEAD = 0;
static const int ALIVE = 1;

// Player sprite
static Texture2D spriteSheetPlayer;
static int frameWidthPlayer = 26;
static int frameHeightPlayer = 25;
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
static void DrawEnemies(Enemy enemies[], size_t size);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messsages
#endif
  
    // Initialize enemies array
    enemies = (Enemy*)malloc(50 * sizeof(Enemy));  // Assuming initial size is 3
    enemies[0] = (Enemy){1, 10, 20, 30.0f, 0, ALIVE};
    enemies[1] = (Enemy){2, 30, 40, 60.0f, 0, ALIVE};
    enemies[2] = (Enemy){3, 50, 60, 90.0f, 0, ALIVE};
    enemies[3] = (Enemy){4, 70, 80, 120.0f, 0, ALIVE};  // Assign values to the new enemy

      
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
        // Scale window to fit the scaled render texture
        SetWindowSize(screenWidth*screenScale, screenHeight*screenScale);                       
        prevScreenScale = screenScale;
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
        
        // Draw water
        for (int y = 0; y < 256; y=y+16) {
            for (int x = 0; x < 256; x=x+16) {
                DrawTextureRec(spriteSheetWater, frameRecWater[waterFrame], (Vector2) { x, y }, WHITE);
            }
        }
        
        
        // debug output stuff
        //DrawText(TextFormat("waterFrame: %d", waterFrame), 10, 10, 20, WHITE);

        // Draw the current player frame
        DrawTextureRec(spriteSheetPlayer, frameRecPlayer[0], (Vector2) { 120, 150 }, WHITE);
        
        
        // DrawEnemies
        DrawEnemies(enemies, sizeof(enemies));
       
        
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


//----------------------------------------------------------------------------------  
// Update and draw enemies
//----------------------------------------------------------------------------------  
void DrawEnemies(Enemy enemies[], size_t size) {
    // Using a for loop for structure iteration

        //printf("ID: %d, X: %d, Y: %d, Alive: %d\n", enemies[i].id, enemies[i].x, enemies[i].y, enemies[i].isAlive);
        
                //DrawTextureRec(spriteSheetPlayer, frameRecPlayer[enemies[i].frame], (Vector2) { enemies[i].x, enemies[i].y }, WHITE);
            for (size_t i = 0; i < size; ++i) {
                
                enemies[i].degrees++;
                
                if (enemies[i].isAlive==1) {
                
                    Rectangle sourceRec = {enemies[i].frame * frameWidthPlayer, 0, frameWidthPlayer, frameHeightPlayer};
                    Vector2 origin = {frameWidthPlayer / 2, frameHeightPlayer / 2}; // Set the origin to the center of the sprite

                    DrawTexturePro(
                        spriteSheetPlayer,
                        sourceRec,
                        (Rectangle){enemies[i].x, enemies[i].y, frameWidthPlayer, frameHeightPlayer},
                        origin,
                        enemies[i].degrees, // Replace with the desired rotation angle (5 degrees in this case)
                        WHITE
                    );
                
                }
            }
                
                
        
    
    
    
}