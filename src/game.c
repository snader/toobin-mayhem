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

    

    // Handle key presses
    if (IsKeyPressed(KEY_SPACE)) {
        switch (currentScreen) {
            case SCREEN_LOGO:
                currentScreen = SCREEN_TITLE;
                break;
            case SCREEN_TITLE:
            
                player.frame = 0;
                player.speed = 0;
                player.degrees = 0;
                player.x = screenWidth / 2;
                player.y = screenHeight / 2;
                player.isAlive = ALIVE;
            
                currentScreen = SCREEN_GAMEPLAY;
                break;
            case SCREEN_GAMEPLAY:
                // Handle gameplay actions
                    //currentScreen = SCREEN_GAMEOVER;
                break;
            case SCREEN_GAMEOVER:
                // Handle gameover actions
                currentScreen = SCREEN_TITLE;
                break;    
            case SCREEN_ENDING:
                break;
            default:
                currentScreen = SCREEN_TITLE;
                break;
        }
    }
    
    if (IsKeyPressed(KEY_Q)) {
        currentScreen = SCREEN_GAMEOVER;
    }

    // Render to texture
    BeginTextureMode(target);
    
    Color backgroundColor = {73, 153, 203, 255}; // #4999cb in RGB
        ClearBackground(backgroundColor);

        switch (currentScreen) {
            case SCREEN_LOGO:
                DrawLogoScreen();
                break;
            case SCREEN_TITLE:
                DrawTitleScreen();
                break;
            case SCREEN_GAMEPLAY:
                UpdateGameplayScreen();
                DrawGameplayScreen();
                break;
            case SCREEN_GAMEOVER:
                UpdateGameoverScreen();
                DrawGameoverScreen();
            case SCREEN_ENDING:
                DrawEndingScreen();
                break;
            default:
                // Handle invalid screen or default case
                break;
        }

    EndTextureMode();

    // Draw scaled texture
    BeginDrawing();
    
        
        ClearBackground(backgroundColor);

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

/**
* UpdateGameplayScreen
*/
void UpdateGameplayScreen(void) {
    // Update variables
    if (frameCount % 30 == 0) {
        waterFrame = GetRandomValue(0, 3);
        
        if (GetRandomValue(1,10)>5) {
            NewDuck(duckies);
        }
    }     
    
    

    UpdatePlayerSprite(&player);  
    UpdateDucks(duckies, player, bullits);
    
    
}

/**
* DrawGameplayScreen
*/
void DrawGameplayScreen(void) {
    // Update logic for the gameplay screen...
    
    
    // Draw water
    for (int y = 0; y < 256; y=y+16) {
        for (int x = 0; x < 256; x=x+16) {
            DrawTextureRec(spriteSheetWater, frameRecWater[waterFrame], (Vector2) { x, y }, WHITE);
        }
    }
    
    //DrawText(TextFormat("Rotation: %f", player.degrees), 100, 100, 20, WHITE);
    //DrawText(TextFormat("bullits: %d", sizeof(bullits)), 100, 130, 20, WHITE);
       
    // Draw Sprites
    //DrawEnemies(enemies, sizeof(enemies));
    

    
    DrawDucks(duckies);
    
    DrawPlayerSprite(&player);
    
    DrawBullits(bullits, sizeof(bullits));
        
    DrawText("Game", 10, 10, 20, GREEN);
        
}


/**
* UpdateGameoverScreen
*/
void UpdateGameoverScreen(void) {
    // Update variables
    if (frameCount % 30 == 0) {
        waterFrame = GetRandomValue(0, 3);
    }     


    if (player.frame<11) {
        player.frame = 11;
        PlaySound(explodingTubeSfx);
         
    } else {
        if (frameCount % 10 == 0) {
            player.frame++;
            if (player.frame==13) {
                PlaySound(splashSfxR);  
            }
            if (player.frame>16) {
                player.isAlive = DEAD;
                
            }
        }     
    }
    //UpdatePlayerSprite(&player);   
}

/**
* DrawGameoverScreen
*/
void DrawGameoverScreen(void) {
    

    // Draw water
    for (int y = 0; y < 256; y=y+16) {
        for (int x = 0; x < 256; x=x+16) {
            DrawTextureRec(spriteSheetWater, frameRecWater[waterFrame], (Vector2) { x, y }, WHITE);
        }
    }
    
    if (player.isAlive) {
        DrawPlayerSprite(&player);
    }
    
    
    DrawText("Game Over", 10, 10, 20, WHITE);
}


// Define update functions for each screen
void DrawLogoScreen(void) {
    // Update logic for the logo screen...
    DrawText("Logo", 10, 10, 20, RED);
}

void DrawTitleScreen(void) {
    // Update logic for the title screen...
    DrawText("Title", 10, 10, 20, RED);
}

void DrawEndingScreen(void) {
    // Update logic for the ending screen...
}