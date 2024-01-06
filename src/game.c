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
            score = 0;
            newHighscore = false;
            player.frame = 0;
            player.speed = 0.4f;
            player.degrees = GetRandomValue(-20,20);
            player.x = screenWidth / 2;
            player.y = (screenHeight / 2) + 30;
            player.isAlive = ALIVE;
            player.energy = 100.0f;    

            for (int i = 1; i < nrOfDucks; i++) {   
                duckies[i].isAlive = DEAD;
            }  
            for (int i = 1; i < nrOfBirds; i++) {   
                birds[i].isAlive = DEAD;
            } 
            for (int i = 1; i < 8; i++) {   
                logs[i].isAlive = DEAD;
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
            player.speed = 0.4f;
            player.degrees = GetRandomValue(-20,20);
            player.x = screenWidth / 2;
            player.y = (screenHeight / 2) + 30;
            
            ducksAdded = 0;      
            birdsAdded = 0;            
            ducksShot = 0;
            player.isAlive = ALIVE;
            
            // onder default
            int tempx = GetRandomValue(30,226); 
            int tempy = GetRandomValue(180,226); ;
            int randnr = GetRandomValue(1,20);
            if (randnr<=5)  { 
                // links
                tempx = GetRandomValue(30,80); 
                tempy = GetRandomValue(50,226); 
            } 
            if (randnr>5 && randnr<=10)  { 
                // rechts
                tempx = GetRandomValue(180,226); 
                tempy = GetRandomValue(50,226); 
            } 
            if (randnr>10 && randnr<=15)  { 
                // boven
                tempx = GetRandomValue(30,226); 
                tempy = GetRandomValue(50,80); 
            }
              
            if (level>0) {
                NewLog(logs, tempx, tempy, 0, 0.2f, GetRandomValue(0,360));
            }
            
            
            
            break;
        case SCREEN_GAMEPLAY:
            
            break;
        case SCREEN_GAMEOVER:
            score = score + (level*10);
            PlaySound(gameOverSfx);
            if (score > highscore) {
                highscore = score;
                newHighscore = true;
            }
            
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
    UpdateMusicStream(music);   // Update music buffer with new stream data
    
    // Get normalized time played for current music stream
        timePlayed = GetMusicTimePlayed(music)/GetMusicTimeLength(music);

      if (timePlayed > 1.0f) timePlayed = 1.0f;   // Make sure time played is no longer than music
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
                //SwitchGameScreen(SCREEN_GAMEPLAY);                
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
    UpdateDucks(duckies, &player, bullits, logs, points);
    UpdateLogs(logs, &player, bullits, points);
    UpdateShit(shit, &player);
    UpdateBirds(birds, &player);
    UpdateExplosions();
    
    if (ducksShot>=levelDuckCount[level] && currentScreen!=SCREEN_LEVEL && frameCount==60) {
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
    DrawLogs(logs);
    DrawPlayerSprite(&player);
    DrawBullits(bullits, sizeof(bullits));
    DrawExplosions();
    DrawFullscreen(overlayTexture);  
    DrawShit(shit);   
    DrawBirds(birds);  
    
     
    DrawEnergyBar(player);
    DrawScores(points);  
    //DrawText(TextFormat("shit: %d", DucksAlive()), 10, 130, 20, WHITE);
        
}

void DrawLevelScreen() {
    
    if (tmpCount == 0) {
        PlaySound(levelSfx);
    }
    tmpCount=tmpCount+2;
    if (tmpCount>256) {
        currentScreen = SCREEN_GAMEPLAY;
        tmpCount=0;
    }
    
     DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, (256-tmpCount)});

    if (tmpCount>25) {
        // Convert the integer to a string using sprintf
        sprintf(levelText, "- LEVEL %d -", level);
        float x = screenWidth/2.0f - MeasureTextEx(konamiFont, levelText, konamiFont.baseSize, 2).x/2.0f; 
        DrawStyledText(levelText, (Vector2){x, 120}, WHITE, BLACK, 1, 1); 
        
        sprintf(levelText, "Shoot %d rubber duckies", levelDuckCount[level]);
        x = screenWidth/2.0f - MeasureTextEx(konamiFont, levelText, konamiFont.baseSize, 2).x/2.0f; 
        DrawStyledText(levelText, (Vector2){x, 135}, WHITE, BLACK, 1, 1); 
    }
    //DrawFullscreen(overlayTexture);    
   
    
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
    UpdateDucks(duckies, &player, bullits, logs, points);
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
    DrawLogs(logs);
    DrawFullscreen(overlayTexture);   
    DrawBirds(birds); 
    
    if (frameCount>20) {       
        DrawStyledText("- GAME OVER -", (Vector2){80, 120}, WHITE, BLACK, 1, 1);     

        
    }
    if (newHighscore) {
            DrawStyledText("- NEW HIGHSCORE -", (Vector2){66, 140}, RED, BLACK, 1, 1);
            
        }
    DrawEnergyBar(player);
    
}


// Define update functions for each screen
void DrawLogoScreen(void) {
    // Update logic for the logo screen...
    Rectangle sourceRec = { 0, 0, raylibTexture.width, raylibTexture.height };
    Rectangle destRec = {10, 10, raylibTexture.width, raylibTexture.height };
    Vector2 origin = { 0, 0 }; // No rotation, use top-left corner as origin
    DrawTexturePro(raylibTexture, sourceRec, destRec, origin, 0.0f, WHITE);
    
    
    DrawTexturePro(toobinTexture, (Rectangle){ 0, 0, toobinTexture.width, toobinTexture.height }, (Rectangle){83, 80, toobinTexture.width, toobinTexture.height }, (Vector2){ 0, 0 }, 0.0f, WHITE);
    
    
    
    DrawText("A game by", 75, 120, 20, BLACK);
    DrawText("Sander Voorn", 55, 150, 20, RED);
    
    DrawStyledText("- UP/DOWN = FORWARD/BACKWARD -", (Vector2){15, 180}, BLACK, WHITE, 1, 0); 
    DrawStyledText("- LEFT/RIGHT = TURN -", (Vector2){52, 195}, BLACK, WHITE, 1,0);
    DrawStyledText("- CTRL = SHOOT -", (Vector2){70, 210}, BLACK, WHITE, 1, 0);
    
    if (frameCount>20) {       
        DrawStyledText("- PRESS SPACE TO CONTINUE -", (Vector2){30, 230}, RED, WHITE, 1, 0);               
    }
    
    // duckies
    Rectangle sourceRecd = {0, 0, 10, 10}; // 
    Vector2 origind = {0,0}; // Set the origin to the center of the sprite     
   
            DrawTexturePro(
                spriteSheetDuck,
                sourceRecd,
                (Rectangle){90, 13, 10, 10},
                origind,
                0,
                WHITE
            );    
}

void DrawTitleScreen(void) {
    // Update logic for the title screen...
    DrawFullscreen(titleTexture);
    
    if (frameCount>20) {       
        DrawStyledText("- PRESS SPACE TO PLAY -", (Vector2){45, 120}, WHITE, BLACK, 1, 1);               
    }
    
    int soundnr = GetRandomValue(1,3);
    if (GetRandomValue(0,100)>70 && frameCount==50 && !IsSoundPlaying(seagullSfx[soundnr])) {
       
        PlaySound(seagullSfx[soundnr]);
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
    