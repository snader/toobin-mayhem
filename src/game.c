#include "raylib.h"
#include "sprites.h"
#include "particle_system.h"
#include "game.h"
#include <math.h>
#include <stdio.h>


void SwitchGameScreen(int GameScreen) 
{
    currentScreen = GameScreen;
    switch (currentScreen) {
        case SCREEN_LOGO:
            
            break;
        case SCREEN_TITLE:
        
            level = 1;
        
            player.frame = 0;
            player.speed = 0.4f;
            player.degrees = GetRandomValue(-10,10);
            player.x = screenWidth / 2;
            player.y = (screenHeight / 2) + 50;
            player.isAlive = ALIVE;
            player.energy = 100.0f;    

            for (int i = 1; i < nrOfDucks; i++) {   
                duckies[i].isAlive = DEAD;
            }            
        
            break;
        case SCREEN_LEVEL:
        
            for (int i = 0; i < 49; i++) { 
                bullits[i].isAlive = DEAD;            
            }
            if (level>1) {
                PlaySound(inflateSfx);
            }
            
            player.energy += 10.0f;
            if (player.energy>100) {
                player.energy = 100.0f;
            }
            ducksAdded = 0;      
            birdsAdded = 0;            
            ducksShot = 0;
            player.isAlive = ALIVE;
            break;
        case SCREEN_GAMEPLAY:
           
            break;
        case SCREEN_GAMEOVER:
            
            
            break;    
        case SCREEN_ENDING:
            break;
        default:
           
            break;
    }
    
}


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
                SwitchGameScreen(SCREEN_TITLE); //currentScreen = SCREEN_TITLE;
                break;
            case SCREEN_TITLE:                           
                SwitchGameScreen(SCREEN_LEVEL);            
                break;
            case SCREEN_LEVEL:
                SwitchGameScreen(SCREEN_GAMEPLAY);                
                break;
            case SCREEN_GAMEPLAY:
                
                break;
            case SCREEN_GAMEOVER:
                SwitchGameScreen(SCREEN_TITLE); 
               
                break;    
            case SCREEN_ENDING:
                break;
            default:
                SwitchGameScreen(SCREEN_TITLE); 
                
                break;
        }
    }
    
    

    // Render to texture
    BeginTextureMode(target);
    
    //Color backgroundColor = {73, 153, 203, 255}; // #4999cb in RGB
        ClearBackground(WHITE);

        switch (currentScreen) {
            case SCREEN_LOGO:
                DrawLogoScreen();
                break;
            case SCREEN_TITLE:
                     
            
                DrawTitleScreen();
                break;
            case SCREEN_LEVEL:
                UpdateGameplayScreen();
                DrawGameplayScreen();
                DrawLevelScreen();
                break;            
            case SCREEN_GAMEPLAY:
                if (IsKeyPressed(KEY_Q) || player.energy <= 0) {
                    SwitchGameScreen(SCREEN_GAMEOVER);
                    player.energy = -10; // in case of Q
                }
            
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
    
        
        ClearBackground(WHITE);

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
        
        int randomCompare = 6;
        if (level>5) {
            randomCompare = 5;
        } else if (level>10) {
            randomCompare = 4;
        }
        
        if (ducksAdded<levelDuckCount[level] && GetRandomValue(1,10)>randomCompare) {
            NewDuck(duckies);            
        }
        if (birdsAdded<1 && GetRandomValue(1,9)>randomCompare) {
            NewBird(birds, &player);            
        }
        if (birdsAdded>0 && GetRandomValue(1,10)>randomCompare) {  //  
            NewShit(shit, birds, &player);
        }
    }     

    UpdatePlayerSprite(&player);  
    UpdateDucks(duckies, &player, bullits);
    UpdateShit(shit, &player);
    UpdateBirds(birds, &player);
    UpdateExplosions();
    
    if (ducksShot>=levelDuckCount[level] && currentScreen!=SCREEN_LEVEL) {
        SwitchGameScreen(SCREEN_LEVEL);
        level++;
    }
      
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
    
    //DrawText(TextFormat("duckies: %d", ducksAdded), 10, 130, 20, WHITE);
    
    DrawDucks(duckies);   
    DrawPlayerSprite(&player);
    DrawBullits(bullits, sizeof(bullits));
    DrawExplosions();
    DrawFullscreen(overlayTexture);  
    DrawShit(shit);   
    DrawBirds(birds);  
     
    DrawEnergyBar(player);
    //DrawText(TextFormat("shit: %d", DucksAlive()), 10, 130, 20, WHITE);
        
}

void DrawLevelScreen() {
    
    tmpCount++;
    if (tmpCount>160) {
        currentScreen = SCREEN_GAMEPLAY;
        tmpCount=0;
    }
    

    // Convert the integer to a string using sprintf
    sprintf(levelText, "- LEVEL %d -", level);
    float x = screenWidth/2.0f - MeasureTextEx(konamiFont, levelText, konamiFont.baseSize, 2).x/2.0f; 
    DrawStyledText(levelText, (Vector2){x, 120}, WHITE, BLACK, 1, 1); 
    
    sprintf(levelText, "Shoot %d rubber duckies", levelDuckCount[level]);
    x = screenWidth/2.0f - MeasureTextEx(konamiFont, levelText, konamiFont.baseSize, 2).x/2.0f; 
    DrawStyledText(levelText, (Vector2){x, 135}, WHITE, BLACK, 1, 1); 
    
    DrawFullscreen(overlayTexture);                
    DrawEnergyBar(player);
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
    UpdateDucks(duckies, &player, bullits);
    UpdateShit(shit, &player);
    UpdateBirds(birds, &player);
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
    
    DrawDucks(duckies);  
    DrawFullscreen(overlayTexture);   
    DrawBirds(birds); 
    
    if (frameCount>20) {       
        DrawStyledText("- GAME OVER -", (Vector2){80, 120}, WHITE, BLACK, 1, 1);               
    }
    DrawEnergyBar(player);
    
}


// Define update functions for each screen
void DrawLogoScreen(void) {
    // Update logic for the logo screen...
    DrawText("Logo", 10, 10, 20, RED);
}

void DrawTitleScreen(void) {
    // Update logic for the title screen...
    DrawFullscreen(titleTexture);
    

    if (frameCount>20) {       
        DrawStyledText("- PRESS SPACE TO PLAY -", (Vector2){45, 120}, WHITE, BLACK, 1, 1);               
    }
}

void DrawEndingScreen(void) {
    // Update logic for the ending screen...
}

void DrawFullscreen(Texture2D myTexture) {
    Rectangle sourceRec = { 0, 0, myTexture.width, myTexture.height };
    Rectangle destRec = { 0, 0, (float)screenWidth, (float)screenHeight };
    Vector2 origin = { 0, 0 }; // No rotation, use top-left corner as origin
    DrawTexturePro(myTexture, sourceRec, destRec, origin, 0.0f, WHITE);
}
    