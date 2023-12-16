#include "raylib.h"
#include "sprites.h"
#include "game.h"

// Update and draw frame
void UpdateDrawFrame(void)
{
    // Update
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

    // Update variables
    if (frameCount % 30 == 0) {
        waterFrame = GetRandomValue(0, 3);
    }

    // Handle key presses
    if (IsKeyPressed(KEY_SPACE)) {
        switch (currentScreen) {
            case SCREEN_LOGO:
                currentScreen = SCREEN_TITLE;
                break;
            case SCREEN_TITLE:
                currentScreen = SCREEN_GAMEPLAY;
                break;
            case SCREEN_GAMEPLAY:
                // Handle gameplay actions
                break;
            case SCREEN_ENDING:
                break;
            default:
                currentScreen = SCREEN_TITLE;
                break;
        }
    }

    // Render to texture
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

    // Draw scaled texture
    BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height }, (Rectangle){ 0, 0, (float)target.texture.width*screenScale, (float)target.texture.height*screenScale }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        // Scanlines
        for (int y = 0; y < target.texture.width*screenScale; y += 4) {
            DrawRectangle(0, y, (float)target.texture.width*screenScale, 1, (Color){0, 0, 0, 25}); // Adjust the alpha value for intensity
        }

        // Draw everything else
    EndDrawing();
}

// *************
// Functions ***
// *************

void UpdateGameplayScreen(void) {
    // Update logic for the gameplay screen...
    
    
    //if (IsKeyDown(KEY_RIGHT)) player.degrees += 1.0f;
    //if (IsKeyDown(KEY_LEFT)) player.degrees -= 1.0f;
         

    UpdatePlayerSprite(&player);
     
    
    
    // Draw water
    for (int y = 0; y < 256; y=y+16) {
        for (int x = 0; x < 256; x=x+16) {
            DrawTextureRec(spriteSheetWater, frameRecWater[waterFrame], (Vector2) { x, y }, WHITE);
        }
    }
    
    //DrawText(TextFormat("Rotation: %f", player.degrees), 100, 100, 20, WHITE);
    //DrawText(TextFormat("Speed: %f", player.speed), 100, 130, 20, WHITE);
       
    // Draw Sprites
    //DrawEnemies(enemies, sizeof(enemies));
    
    DrawPlayerSprite(&player);
    
    //DrawText(TextFormat("ripples: %f", sizeof(ripples) / sizeof(ripples[0])), 100, 130, 20, WHITE);
    
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