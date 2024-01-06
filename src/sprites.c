// sprites.c

#include "sprites.h"
#include "particle_system.h"
#include <math.h>

// Include necessary declarations from main.c
extern Texture2D spriteSheetPlayer;
extern Texture2D spriteSheetRipple;
extern Texture2D spriteSheetDuck;
extern Texture2D spriteSheetBirdWhite;
extern Texture2D spriteSheetBirdBlue;
extern Texture2D spriteSheetLog;
extern Texture2D spriteCounter;
extern Texture2D explosionTexture;
extern int frameWidthPlayer;
extern int frameHeightPlayer;
extern float frameCountPlayer;
extern Sound splashSfxL;
extern Sound splashSfxR;
extern Sound quackSfx[4];
extern Sound seagullSfx[4];
extern Sound fartSfx[4];
extern Sound ouchSfx[3];
extern Sound popSfx;
extern Sound ewwSfx;
extern Rectangle frameRecPlayer[];
extern Rectangle frameRecLog[];
extern int DEAD;
extern int ALIVE;
extern int DEFAULT;
extern int SHOOTING;
extern int EXPLODING;
extern int duckHit;
extern Sprite* ripples;
extern Sprite* bullits;
extern Sprite* duckies;
extern Sprite* birds;
extern Sprite* shit;
extern int frameCount;
extern int myCount;
extern int popperSoundNr; 
extern int screenWidth;
extern int screenHeight;



/**
* DrawPlayerSprite
*/
void DrawPlayerSprite(Sprite *sprite) {
    
    int tempx, tempy;
    
    DrawRipples(ripples, sizeof(ripples));
       
    
    sprite->drawFrame = sprite->frame;
    if (IsKeyDown(KEY_LEFT)) {
        if (sprite->frame==1) {
            //sprite->drawFrame = 3;
            sprite->drawFrame = 5;
        }
        if (sprite->frame==2) {
            //sprite->drawFrame = 4;
            sprite->drawFrame = 6;
        }
    }
    if (IsKeyDown(KEY_RIGHT)) {
        if (sprite->frame==1) {
            //sprite->drawFrame = 5;
            sprite->drawFrame = 3;
        }
        if (sprite->frame==2) {
            //sprite->drawFrame = 6;
            sprite->drawFrame = 4;
        }
    } 
    
    if (sprite->drawFrame==1 || sprite->drawFrame==5) {
                       
        // right ripple
        tempx = sprite->x - cos(sprite->degrees * DEG2RAD) * 12;
        tempy = sprite->y - sin(sprite->degrees * DEG2RAD) * 11;
        NewRipple(ripples, sizeof(ripples), tempx, tempy);
    }
    if (sprite->drawFrame==1 || sprite->drawFrame==3) {
        // left ripple
        tempx = sprite->x + cos(sprite->degrees * DEG2RAD) * 12;
        tempy = sprite->y + sin(sprite->degrees * DEG2RAD) * 11;
        NewRipple(ripples, sizeof(ripples), tempx, tempy);
    }
    
    Rectangle sourceRec = {sprite->drawFrame * frameWidthPlayer, 0, frameWidthPlayer, frameHeightPlayer};
    Vector2 origin = {frameWidthPlayer / 2, frameHeightPlayer / 2}; // Set the origin to the center of the sprite  

    DrawTexturePro(
        spriteSheetPlayer,
        sourceRec,
        (Rectangle){sprite->x+2, sprite->y+3, frameWidthPlayer, frameHeightPlayer},
        origin,
        sprite->degrees,
        Fade(BLACK, 0.15f)
    );


    DrawTexturePro(
        spriteSheetPlayer,
        sourceRec,
        (Rectangle){sprite->x, sprite->y, frameWidthPlayer, frameHeightPlayer},
        origin,
        sprite->degrees,
        WHITE
    );
    
    //DrawText(TextFormat("Xp: %.2f", sprite->x), 10, 70, 20, WHITE);
    
    //DrawCircleLines(sprite->x , sprite->y , 10, WHITE);
    //DrawCircleLines(sprite->x , sprite->y , 20, WHITE);
    
    //DrawCircleV((Vector2){sprite->x , sprite->y} , 2, WHITE);
    /*
        // above player, tonen als y>0 en/of x>0
        Vector2 circlePos;
        circlePos.x = sprite->x + sin(sprite->degrees * DEG2RAD) * 16;
        circlePos.y = sprite->y - cos(sprite->degrees * DEG2RAD) * 16;
        // Draw the rotating circle below the sprite
        //DrawCircleV(circlePos, 2, WHITE);
        
        // right
        circlePos.x = sprite->x + cos(sprite->degrees * DEG2RAD) * 12;
        circlePos.y = sprite->y + sin(sprite->degrees * DEG2RAD) * 11;
        //DrawCircleV(circlePos, 2, WHITE);
        
        // left
        circlePos.x = sprite->x - cos(sprite->degrees * DEG2RAD) * 12;
        circlePos.y = sprite->y - sin(sprite->degrees * DEG2RAD) * 11;       
        //DrawCircleV(circlePos, 2, WHITE);
    */    
        // below player, tonen als y<0 en/of x<0
        
        //circlePos.x = sprite->x - sin(sprite->degrees * DEG2RAD) * 16;
        //circlePos.y = sprite->y + cos(sprite->degrees * DEG2RAD) * 16;
        // Draw the rotating circle below the sprite
        //DrawCircleV(circlePos, 2, WHITE);
        
        //DrawText(TextFormat("energy: %f", sprite->energy), 10, 160, 20, WHITE);
}

/**
* UpdatePlayerSprite
*/
void UpdatePlayerSprite(Sprite *sprite) {
            
    sprite->frameDelay++;
    
    if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN)) && !IsKeyDown(KEY_LEFT_CONTROL)) {
                
        if (sprite->frameDelay>10) {
            if (IsKeyDown(KEY_UP)) {
                sprite->frame--;                
            } else if (IsKeyDown(KEY_DOWN)) {
                sprite->frame++;
            }
            
            sprite->frameDelay=0;
            
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN)) {
                
                if (sprite->frame == 1) {                
                    PlaySound(splashSfxL);      
                    sprite->energy -= (0.05f + (level/10.0f));
                } 
                
            }
        
            
        }
        
        if (sprite->frame>2) {
            sprite->frame = 0;
        }
        if (sprite->frame<0) {
            sprite->frame = 2;
        }
        if (sprite->frame<0) {}
        if (sprite->frame==1) {
            if (IsKeyDown(KEY_UP)) {
                sprite->speed += 0.04f;
            } else if (IsKeyDown(KEY_DOWN)) {
                sprite->speed -= 0.04f;
            }
        }
        
        
    }
    
    if (sprite->speed < -0.02f || sprite->speed > 0.02f){
        if (IsKeyDown(KEY_LEFT)) {
            sprite->degrees -= 1.0f;
        } else if (IsKeyDown(KEY_RIGHT)) {
            sprite->degrees += 1.0f;
        }
    }
    
    if (!IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP) && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) 
        && sprite->frame!=7 && sprite->frame!=8) {
        sprite->frame = 0;
        if (sprite->frameDelay>10) {
            //sprite->energy += 0.015f;
            //if (sprite->energy>100) { sprite->energy = 100.0f;}
        }
        
    }
    
    if (sprite->degrees > 360) {
      sprite->degrees = 0.0f;
    } else if (sprite->degrees < 0) {
      sprite->degrees = 360.0f;
    }
    
    if (sprite->speed>1.0f) {
        sprite->speed=1.0f;
    }
    if (sprite->speed<-1.0f) {
        sprite->speed=-1.0f;
    }
    
    if (sprite->speed>0) {
        sprite->speed -= 0.01f;
    }
    if (sprite->speed<0) {
        sprite->speed += 0.01f;
    }
       
    sprite->x += sprite->speed * sin(sprite->degrees * DEG2RAD);
    sprite->y += sprite->speed * -cos(sprite->degrees * DEG2RAD);
    
    //
    // Clamp the sprite within the screen bounds 

    int doScratch = 0;
    if (sprite->x > 243) {
        sprite->x = 243;  
        doScratch = 1;
            
    }
    if (sprite->x < 13) {
        sprite->x = 13;
        doScratch = 1;
        
    }
    if (sprite->y < 38) {
        sprite->y = 38;
        doScratch = 1;
      
    }
    if (sprite->y > 243) {
        sprite->y = 243;
        doScratch = 1;
    }
    if (doScratch ==1 && !IsSoundPlaying(scratchSfx[2]) && sprite->speed > 0.5f  && sprite->frameDelay==10) {           
        PlaySound(scratchSfx[2]);      
        sprite->energy -= 1; 
    }          
          

    ////
    
    // shooting - bullits
    if (sprite->frame == 8 && sprite->frameDelay>30) {        
        sprite->frame = 0;        
    }    
    if (sprite->frame == 7) {
        sprite->frameDelay = 0;
        sprite->frame = 8;     
    }   
    if (IsKeyPressed( KEY_LEFT_CONTROL)) {
        NewBullit(bullits, sizeof(bullits), sprite->x, sprite->y, sprite->degrees); 
        
        sprite->speed = sprite->speed+0.1;
        sprite->frame = 7;      
        sprite->degrees = sprite->degrees+1;        
    }
    
    if (frameCount==59) {
        sprite->energy -= 0.1f;
    }
    
}

/**
* DrawScores
*/
void DrawScores(Sprite points[]) {
    
   
    for (int i = 0; i < 25; i++) {                                 
                       
        if (points[i].isAlive == ALIVE) {        
           
           if (points[i].counter<0) {
               DrawTextEx(konamiFont, TextFormat("%d", points[i].counter), (Vector2){points[i].x, points[i].y}, points[i].frame, 2.0f, RED);
           } else {
               DrawTextEx(konamiFont, TextFormat("%d", points[i].counter), (Vector2){points[i].x, points[i].y}, points[i].frame, 2.0f, WHITE);
           }
           
           points[i].frame += 1.5f;
           points[i].y -= 1.0f;    
           points[i].x -= 0.3f;
                    
        }
        
        if (points[i].frame > 25) {
            points[i].isAlive = DEAD;
        } 
        
    }
    
}

/**
* DrawBullits
*/
void DrawBullits(Sprite bullits[], size_t size) {
    
    
    for (int i = 0; i < 49; i++) {                                 
                       
        if (bullits[i].isAlive == ALIVE) {
           
           bullits[i].x += bullits[i].speed * sin(bullits[i].degrees * DEG2RAD);
           bullits[i].y += bullits[i].speed * -cos(bullits[i].degrees * DEG2RAD);                    
         
           DrawPixel(bullits[i].x, bullits[i].y, WHITE); 
        }
        
        if (bullits[i].x > 256 || bullits[i].x < 1 || bullits[i].y > GetScreenHeight() || bullits[i].y < 1) {
            bullits[i].isAlive = DEAD;
        }
        
    }
    
}

/*
*
*/
void NewBullit(Sprite bullits[], size_t size, int x, int y, int degrees) {
    
    int tempx, tempy;
    for (int i = 0; i < 49; i++) {
        // reuse dead bullit       

        if (bullits[i].isAlive == DEAD) {
            
            tempx = x - cos(degrees * DEG2RAD) * 8;
            tempy = y - sin(degrees * DEG2RAD) * 11;            
            
            bullits[i].isAlive = ALIVE;
            bullits[i].x = tempx;
            bullits[i].y = tempy;
            bullits[i].frame = 0;
          
            bullits[i].degrees = fmod(degrees + 180, 360) + GetRandomValue(-2,2);
            bullits[i].speed = 2;
            popperSoundNr++;
            if (popperSoundNr>3) {
                popperSoundNr=1;
            }
            PlaySound(popperSfx[popperSoundNr]);
            //DrawText(TextFormat("popperSoundNr: %d", popperSoundNr), 10, 130, 20, WHITE);
            break;
        }
        
        
    }
}

/*
*
*/
void NewScores(Sprite points[], int x, int y, int score) {
    
  
    for (int i = 0; i < 25; i++) {
        // reuse dead score       

        if (points[i].isAlive == DEAD) {                               
            
            points[i].isAlive = ALIVE;
            points[i].x = x;
            points[i].y = y;
            points[i].frame = 1;
            points[i].counter = score;
                      
            break;
        }
        
        
    }
}

/**
*
*/
void DrawRipples(Sprite ripples[], size_t size) {
    for (size_t i = 0; i < size; ++i) {                              
               
        if (ripples[i].isAlive == ALIVE) {
            
            ripples[i].drawFrame = (ripples[i].frame / 10) % 4;
            
            Rectangle sourceRec = {ripples[i].drawFrame * 11, 0, 11, 11}; // 
            Vector2 origin = {11 / 2, 11 / 2}; // Set the origin to the center of the sprite

            DrawTexturePro(
                spriteSheetRipple,
                sourceRec,
                (Rectangle){ripples[i].x, ripples[i].y, 11, 11},
                origin,
                ripples[i].degrees,
                WHITE
            );           
            
            ripples[i].frame++;
            
            if (ripples[i].frame>30) {
                ripples[i].isAlive = DEAD;
                ripples[i].frame=0;
            }
            
        }
        
        
    }
}

/**
*
*/
void NewRipple(Sprite ripples[], size_t size, int x, int y) {
    for (size_t i = 0; i < size; ++i) {
        // reuse dead ripple        
        
        if (ripples[i].isAlive == DEAD) {
            ripples[i].isAlive = ALIVE;
            ripples[i].x = x;
            ripples[i].y = y;
            ripples[i].frame = 0;
            break;
        }
    }
}


/**
*
*/
void DrawDucks(Sprite duckies[]) {
    for (int i = 1; i < nrOfDucks; i++) {                                 
                       
        if (duckies[i].isAlive == ALIVE) {
            
            // Check angle to determine frame
            if (duckies[i].degrees >= 55 && duckies[i].degrees < 125) {
                // Angle between 45 and 135 degrees (moving up)
                duckies[i].drawFrame = 4;
            } else if (duckies[i].degrees >= 125 && duckies[i].degrees < 215) {
                // Angle between 135 and 225 degrees (moving left)
                duckies[i].drawFrame = 1;
            } else if (duckies[i].degrees >= 215 && duckies[i].degrees < 325) {
                // Angle between 225 and 315 degrees (moving down)
                duckies[i].drawFrame = 2;
            } else {
                // All other angles (moving right)
                duckies[i].drawFrame = 3;
            }
            
            Rectangle sourceRec = {duckies[i].drawFrame * 10, 0, 10, 10}; // 
            Vector2 origin = {10 / 2, 10 / 2}; // Set the origin to the center of the sprite                   

            DrawTexturePro(
                spriteSheetDuck,
                sourceRec,
                (Rectangle){duckies[i].x+1, duckies[i].y+2, 10, 10},
                origin,
                0,
                Fade(BLACK, 0.15f)
            );  
            DrawTexturePro(
                spriteSheetDuck,
                sourceRec,
                (Rectangle){duckies[i].x, duckies[i].y, 10, 10},
                origin,
                0,
                WHITE
            );        

            if (duckies[i].state == EXPLODING) {
                
                 Rectangle sourceRecb = {duckies[i].counter * 5, 0, 5, 5}; // 
                 Vector2 originb = {5 / 2, 5 / 2}; 
                
                DrawTexturePro(
                    spriteCounter,
                    sourceRecb,
                    (Rectangle){duckies[i].x -1, duckies[i].y -9, 5, 5},
                    originb,
                    0,
                    WHITE
                ); 
                
                if (duckies[i].counter == 0 && frameCount>55) {
                    Rectangle sourceRec = { 0, 0, explosionTexture.width, explosionTexture.height };
                    Rectangle destRec = { duckies[i].x - 5, duckies[i].y - 5, (float)explosionTexture.width, (float)explosionTexture.height };
                    Vector2 origin = { 0, 0 }; // No rotation, use top-left corner as origin
                    DrawTexturePro(explosionTexture, sourceRec, destRec, origin, 0.0f, WHITE);
                }
            }
            
          // DrawCircleLines(duckies[i].x , duckies[i].y , 5, WHITE);
            
            //DrawText(TextFormat("duckie: %d", duckies[i].counter), 10, 130, 20, WHITE);
           // if (ripples[i].frame>30) {
           //     ripples[i].isAlive = DEAD;
           //     ripples[i].frame=0;
            //}
            
        }
        
        
    }
}

int DucksAlive() {
    int count = 0;
    for (int i = 1; i < nrOfDucks; i++) {
        if (duckies[i].isAlive == ALIVE) {
            count++;
        }
    }
    
    
    return count;
        
}

void UpdateDucks(Sprite duckies[], Sprite *player, Sprite bullits[], Sprite logs[], Sprite points[]) {
    for (int i = 1; i < nrOfDucks; i++) {
                
        if (duckies[i].isAlive == ALIVE) {
            
            // ducks at the borders of the screen
            if (duckies[i].x > 250 || duckies[i].x < 5 || duckies[i].y < 30 || duckies[i].y > 250) { 
                duckies[i].degrees += 180.0f;
                if (duckies[i].degrees >= 360.0f) {
                   duckies[i].degrees -= 360.0f;
                }
            }              
            
            if (player->energy<=0) {
                duckies[i].state = DEFAULT; // for game over, ducks go in rest mode
            } else {
                        
                if (frameCount % 60 == 0) {
                    duckies[i].secondsAlive++;      
                    
                    if (duckies[i].state == EXPLODING) {
                        duckies[i].counter--;
                        if (duckies[i].counter==0) {
                            PlaySound(quackSfx[GetRandomValue(1,3)]);
                        }
                        if (duckies[i].counter<0) {
                            duckies[i].isAlive = DEAD;
                            
                            // not counting for amount shot total
                            ducksAdded--;
                            score-=2;
                            NewScores(points, duckies[i].x, duckies[i].y - 10, -2);
                            if (score<0) {
                                score = 0;
                            }
                                                                                    
                            PlaySound(popSfx);
                            Vector2 explosionPosition = {duckies[i].x, duckies[i].y};  
                            InitializeExplosion(explosionPosition);                        
                            Vector2 playerPosition = {player->x, player->y};  // Replace with actual player position

                            int distance = calculateDistance(explosionPosition, playerPosition);
                            if (distance < 25) {
                                player->energy = player->energy - ((duckHit-distance)*0.6f);
                                if (distance<10) {
                                    PlaySound(ouchSfx[2]);
                                } else {
                                    PlaySound(ouchSfx[1]);
                                }
                                
                            }
                        }
                    }
                    
                                    
                }
            
            }
            
            // Check for collision between player and duck 
            if (player->energy>0) { 
            
                if (CheckCollisionCircles((Vector2){duckies[i].x, duckies[i].y}, 3, (Vector2){player->x, player->y}, 18)) {
                                       
                    if (GetRandomValue(1,100)>50) {
                        duckies[i].degrees = player->degrees + GetRandomValue(-15,15);
                    }
                    
                    if (duckies[i].state != EXPLODING) {
                        duckies[i].speed = player->speed - (player->speed/5);
                        if (duckies[i].speed<0.6) { duckies[i].speed=0.6; }
                        duckies[i].x += (player->speed*2) * sin(duckies[i].degrees * DEG2RAD);
                        duckies[i].y += (player->speed*2) * -cos(duckies[i].degrees * DEG2RAD);  
                    }
                    
                    if (GetRandomValue(1,100)>95) {
                        PlaySound(quackSfx[GetRandomValue(1,3)]);
                    }
                    
                }
                
                // lifespan and suicidal ducks.. based on the lifespan and level
                if (duckies[i].state == DEFAULT && duckies[i].secondsAlive > ((GetRandomValue(100,200)-level)/10)) {
                    duckies[i].counter = 4;
                    duckies[i].state = EXPLODING;      
                    duckies[i].speed = GetRandomValue(2,(10+level)) / 10.0f;    
                    
                   
                }
                
                // make duckies follow
                if (duckies[i].state == EXPLODING && GetRandomValue(1,10)>8) {
                    duckies[i].degrees = GetRandomValue(-10,+10) + 90.0f + atan2(player->y - duckies[i].y, player->x - duckies[i].x) * RAD2DEG;
                    
                }
            
            
            
                for (int b = 0; b < 50; b++) {
                    
                    if (bullits[b].isAlive == ALIVE) {
                        if (CheckCollisionPointCircle((Vector2){bullits[b].x, bullits[b].y}, (Vector2){duckies[i].x, duckies[i].y}, 4)) {
                            duckies[i].isAlive = DEAD;
                            bullits[b].isAlive = DEAD;
                            ducksShot++;
                            score = score + 10;
                            NewScores(points, duckies[i].x, duckies[i].y - 10, 10);
                            Vector2 explosionPosition = {bullits[b].x, bullits[b].y};  
                            InitializeExplosion(explosionPosition);  
                            PlaySound(quackSfx[GetRandomValue(1,3)]);
                        }   
                    }
                    
                    
                }
            }
            
            bool reverse = false;
            // duck collision with logs
            for (int i = 1; i < 8; i++) {
                
                if (logs[i].isAlive == ALIVE) {
                    if (CheckCollisionCircles((Vector2){logs[i].x, logs[i].y}, frameRecLog[logs[i].frame].height/2, (Vector2){duckies[i].x, duckies[i].y}, 8)) {
                        duckies[i].degrees += 180.0f + GetRandomFloat(-10.0f, 10.0f);
                        if (duckies[i].degrees >= 360.0f) {
                           duckies[i].degrees -= 360.0f;
                        }
                        reverse = true;
                    }
                }
            }
            
            // reverse
            if (!reverse) {
                if (
                (duckies[i].secondsAlive > 5 && GetRandomValue(0,100) < 5) && ( (duckies[i].x < GetRandomValue(5,15) || duckies[i].x > GetRandomValue(240,256) || 
                    duckies[i].y < GetRandomValue(5,15) || duckies[i].y > GetRandomValue(240,256) ) )
                    ) {
                        duckies[i].degrees += 180.0f + GetRandomFloat(-10.0f, 10.0f);
                        if (duckies[i].degrees >= 360.0f) {
                           duckies[i].degrees -= 360.0f;
                        }
                }
            }
            
            // move the duckies
            duckies[i].x += duckies[i].speed * sin(duckies[i].degrees * DEG2RAD);
            duckies[i].y += duckies[i].speed * -cos(duckies[i].degrees * DEG2RAD);   
            
            if (duckies[i].speed > 0.5) {
                duckies[i].speed -= 0.05;
            }
            
           
            
            
        }
        
        
    }
    
}


/*
*
*/
void NewDuck(Sprite duckies[]) {
    
    int tempx, tempy;
    int tempr = GetRandomValue(-10,10);
    
    float randomFloat = (float)GetRandomValue(1, 3) / 10;
    
    for (int i = 1; i < nrOfDucks; i++) {
        // reuse dead duck       

        if (duckies[i].isAlive == DEAD) {
                       
            duckies[i].secondsAlive = 0;
            duckies[i].speed = randomFloat;            
            
            if (tempr>5) {
                tempx = 5;                
                tempy = GetRandomValue(30,236);   
                duckies[i].degrees = GetRandomValue(10,170);                 
            } else if (tempr>0) {
                tempx = 250;                
                tempy = GetRandomValue(30,236);  
                duckies[i].degrees = GetRandomValue(190,350);                    
            } else if (tempr<-5) {
                tempx = GetRandomValue(20,236);                  
                tempy = 30;   
                duckies[i].degrees = GetRandomValue(100,270); 
            } else {   
                if (GetRandomValue(1,10)>5) {
                    duckies[i].degrees = GetRandomValue(280,355); 
                } else {
                    duckies[i].degrees = GetRandomValue(5,85); 
                }
                tempx = GetRandomValue(20,236);
                tempy = 250; 
            }        
            
            duckies[i].isAlive = ALIVE;
            duckies[i].x = tempx;
            duckies[i].y = tempy;
            duckies[i].frame = 0;
            duckies[i].state = DEFAULT;
            duckies[i].counter = 5;
            
            ducksAdded++;
            
           // 
            //PlaySound(popperSfx[popperSoundNr]);
            
            break;
        }
        
        
    }
}


void NewLog(Sprite logs[], int x, int y, int size, float speed, float degrees) {
    

    for (int i = 1; i < 8; i++) {
           

        if (logs[i].isAlive == DEAD) {
            if (size==0) {
                logs[i].frame = 0; 
            } else {
                logs[i].frame = GetRandomValue(1,2);
            }
            logs[i].isAlive = ALIVE;
            logs[i].x = x;
            logs[i].y = y;
            logs[i].degrees = degrees;
            logs[i].speed = speed; //GetRandomFloat(-0.005f, 0.005f);
            //if (randomnr == 1) { logs[i].degrees=90; }
            //if (randomnr == 2) { logs[i].degrees=180; }
            //if (randomnr == 3) { logs[i].degrees=270; }
            
            break;
        }
    }
    
}

/*
*
*/
void NewBird(Sprite birds[], Sprite *player) {
    
    int tempx, tempy;
    int tempr = GetRandomValue(-10,10);
    
    float randomFloat = (float)GetRandomValue(6, 9) / 10;
    
    for (int i = 1; i < nrOfBirds; i++) {
        // reuse dead bird       

        if (birds[i].isAlive == DEAD) {
                       
            birds[i].secondsAlive = 0;
            birds[i].speed = randomFloat; 

            tempy = player->y - 40 + GetRandomValue(-5,5);            
            
            if (tempr>0) {
                tempx = -20;                
                birds[i].frame = 2;   
                birds[i].degrees = GetRandomValue(70,120);                 
            } else {
                tempx = 280;                
                birds[i].frame = 0;
                birds[i].degrees = GetRandomValue(250,300);                    
            
            }        
            
            birds[i].isAlive = ALIVE;
            birds[i].x = tempx;
            birds[i].y = tempy;
            
            birds[i].state = GetRandomValue(0,1);
            birds[i].counter = 5;
            
            birdsAdded++;
 
            
            break;
        }
        
        
    }
}

/*
*
*/
void NewShit(Sprite shit[], Sprite birds[], Sprite *player) {
    
    int randomnr;
   for (int j = 1; j < nrOfBirds; j++) {
       
       if (birds[j].isAlive == ALIVE && birds[j].x>(player->x - 30) && birds[j].x<(player->x +30)) { // 
       
            for (int i = 1; i < nrOfBirds; i++) {
                // reuse dead shit       
                if (shit[i].isAlive == DEAD) {
                               
                    shit[i].secondsAlive = 0;
                    shit[i].speed = birds[j].speed; 
                    shit[i].degrees = birds[j].degrees; 
                                
                    shit[i].isAlive = ALIVE;
                    shit[i].x = birds[j].x;
                    shit[i].y = birds[j].y;
                    
                    if (GetRandomValue(0,10)>3) {
                        randomnr = GetRandomValue(1,3);
                        if (!IsSoundPlaying(fartSfx[randomnr])) {
                            PlaySound(fartSfx[randomnr]);  
                        }                            
                    }
                    break;
                }
      
            }
       }
       
       
   }
   

  
}


void UpdateShit(Sprite shit[], Sprite *player) {
    for (int i = 1; i < nrOfBirds; i++) {
                
        if (shit[i].isAlive == ALIVE) {
            
            shit[i].secondsAlive++;
            shit[i].speed -= 0.1f;
            if (shit[i].speed < 0) {
                shit[i].speed = 0;
            }
            
            // move the shit
            shit[i].x += shit[i].speed * sin(shit[i].degrees * DEG2RAD);
            shit[i].y += shit[i].speed * -cos(shit[i].degrees * DEG2RAD);    

            shit[i].y += 0.6f;
            
            Vector2 shitPosition = {shit[i].x, shit[i].y};                         
            Vector2 playerPosition = {player->x, player->y};
            int distance = calculateDistance(shitPosition, playerPosition);
                          
            
            if (shit[i].secondsAlive>30 && distance<15 ) {
                
                if (!IsSoundPlaying(ewwSfx)) {
                    PlaySound(ewwSfx);
                }
                player->speed = player->speed / 3;
                player->energy -= 3;
                shit[i].isAlive = DEAD; 
            }
            
 
            if (shit[i].secondsAlive > 80) { 
                shit[i].isAlive = DEAD;                 
            }           
                 
            
            
        }     
        
    }
    
}

void UpdateLogs(Sprite logs[], Sprite *player, Sprite bullits[], Sprite points[]) {
    for (int i = 1; i < 8; i++) {
                
        if (logs[i].isAlive == ALIVE) {
            
            if (logs[i].speed>0) {
                logs[i].speed = logs[i].speed - 0.001f;
            }
                                              
                
         
            // collision with player
            if (CheckCollisionCircles((Vector2){logs[i].x, logs[i].y}, frameRecLog[logs[i].frame].height/2, (Vector2){player->x, player->y}, 10)) {
                player->energy = player->energy-10;
            }
            
            if (CheckCollisionCircles((Vector2){logs[i].x, logs[i].y}, frameRecLog[logs[i].frame].height/2, (Vector2){player->x, player->y}, 8)) {
                                                        
                player->energy = -10; 
                               
            }
            
            // collision with bullits                   
            for (int b = 0; b < 50; b++) {
                    
                if (bullits[b].isAlive == ALIVE) {
                    if (CheckCollisionPointCircle((Vector2){bullits[b].x, bullits[b].y}, (Vector2){logs[i].x, logs[i].y}, frameRecLog[logs[i].frame].height/2)) {
                        logs[i].isAlive = DEAD;
                        bullits[b].isAlive = DEAD;
                        
                        if (logs[i].frame==0) {
                            
                            float angle = atan2f(bullits[b].y - logs[i].y, bullits[b].x - logs[i].x);
                            // Calculate the new velocities after the collision // obj1 = log
                        
                            float obj2NewSpeed = bullits[b].speed / 10;

                                                        
                            float overlap = 2 * 2 - calculateDistance((Vector2){logs[i].x, bullits[b].y}, (Vector2){logs[i].x, bullits[b].y});
                            
                            int log1x = logs[i].x;
                            int log1y = logs[i].y;
                            int log2x = bullits[b].x;
                            int log2y = bullits[b].y;
                                                        
                            log1x -= cos(angle) * overlap / 2;
                            log1y -= sin(angle) * overlap / 2;
                            log2x += cos(angle) * overlap / 2;
                            log2y += sin(angle) * overlap / 2;

                            // Update the angles based on the new velocities
                            float log1degrees = atan2f(sin(logs[i].degrees * DEG2RAD - angle), cos(logs[i].degrees * DEG2RAD - angle)) * RAD2DEG;
                            float log2degrees = atan2f(sin(bullits[b].degrees * DEG2RAD - angle), cos(bullits[b].degrees * DEG2RAD - angle)) * RAD2DEG;
                            
                            score = score + 5;
                            NewScores(points, logs[i].x, logs[i].y - 10, 5);
                            NewLog(logs, log1x, log1y, 1, obj2NewSpeed, log1degrees);
                            NewLog(logs, log2x, log2y, 2, obj2NewSpeed, log2degrees);
                            
                            
                            
                        } else {
                            score = score + 10;
                            NewScores(points, logs[i].x, logs[i].y - 10, 10);
                        }
                        
                        Vector2 explosionPosition = {logs[i].x, logs[i].y};  
                        InitializeExplosion(explosionPosition);   
                        PlaySound(woodSfx);
                    }   
                }
                    
                    
            }
            
                     
             // move the log
            logs[i].x += logs[i].speed * sin(logs[i].degrees * DEG2RAD);
            logs[i].y += logs[i].speed * -cos(logs[i].degrees * DEG2RAD);   
            
            
            if (logs[i].x > 286 || logs[i].x < -20 || logs[i].y<-20 || logs[i].y>286) {
                logs[i].isAlive = DEAD;
            }     
            
        }  

               
        
    }
    
}

void UpdateBirds(Sprite birds[], Sprite *player) {
    
    int soundnr;
    for (int i = 1; i < nrOfBirds; i++) {
                
        if (birds[i].isAlive == ALIVE) {
            
            // ducks at the borders of the screen
            if (birds[i].x > 290 || birds[i].x < -40 || birds[i].y < -40 || birds[i].y > 290) { 
                birds[i].isAlive = DEAD;  
                birdsAdded--;
            }              
                 
            // move the birds
            birds[i].x += birds[i].speed * sin(birds[i].degrees * DEG2RAD);
            birds[i].y += birds[i].speed * -cos(birds[i].degrees * DEG2RAD);    

            soundnr = GetRandomValue(1,3);
            if (GetRandomValue(0,100)>70 && frameCount==50 && !IsSoundPlaying(seagullSfx[soundnr])) {
               
                PlaySound(seagullSfx[soundnr]);
            }
            
        }     
        
    }
    
    
    
}


/**
*
*/
void DrawBirds(Sprite birds[]) {
    
    int myCount = 0;
    for (int i = 1; i < nrOfBirds; i++) {                                 
                       
        if (birds[i].isAlive == ALIVE) {
            
            myCount++;
            
            if (frameCount > 40) {    
                if (birds[i].frame==0) {         
                    birds[i].frame = 1;                               
                }
                if (birds[i].frame==2) {          
                    birds[i].frame = 3;                               
                }
            } else {                
                if (birds[i].frame==1) {           
                    birds[i].frame = 0;                               
                }     
                if (birds[i].frame==3) {            
                    birds[i].frame = 2;                               
                }                
            }         
            
            birds[i].drawFrame = birds[i].frame;

            
            Rectangle sourceRec = {birds[i].drawFrame * 18, 0, 18, 16}; // 
            Vector2 origin = {18 / 2, 16 / 2}; // Set the origin to the center of the sprite                   

            if (birds[i].state == 1) {
                DrawTexturePro(
                    spriteSheetBirdWhite,
                    sourceRec,
                    (Rectangle){birds[i].x + 15, birds[i].y + 50, 18, 16},
                    origin,
                    0,
                    Fade(BLACK, 0.15f)
                ); 
                DrawTexturePro(
                    spriteSheetBirdWhite,
                    sourceRec,
                    (Rectangle){birds[i].x, birds[i].y, 18, 16},
                    origin,
                    0,
                    WHITE
                );        
            } else {
                DrawTexturePro(
                    spriteSheetBirdBlue,
                    sourceRec,
                    (Rectangle){birds[i].x + 15, birds[i].y + 50, 18, 16},
                    origin,
                    0,
                    Fade(BLACK, 0.15f)
                ); 
                DrawTexturePro(
                    spriteSheetBirdBlue,
                    sourceRec,
                    (Rectangle){birds[i].x, birds[i].y, 18, 16},
                    origin,
                    0,
                    WHITE
                );   
            }

            //DrawText(TextFormat("Xb: %.2f", birds[i].x), 10, 50, 20, WHITE);
            
          // DrawCircleLines(duckies[i].x , duckies[i].y , 5, WHITE);
            
           // DrawText(TextFormat("bird: %d", myCount), 10, 150, 20, WHITE);
           // if (ripples[i].frame>30) {
           //     ripples[i].isAlive = DEAD;
           //     ripples[i].frame=0;
            //}
            
        }
        
        
    }
}


void DrawShit(Sprite shit[]) {
    
    int myCount = 0;
    for (int i = 1; i < nrOfBirds; i++) {                                 
                       
        if (shit[i].isAlive == ALIVE) {
            myCount++;
            DrawCircle(shit[i].x , shit[i].y , 1, LIGHTGRAY);
          
            
        }
        
        
    }
    //DrawText(TextFormat("shit: %d", myCount), 10, 150, 20, WHITE);
}



void DrawLogs(Sprite logs[]) {
    
    int myCount = 0;
    for (int i = 1; i < 8; i++) {                                 
                       
        if (logs[i].isAlive == ALIVE) {
            myCount++;
           
            
            Vector2 origin = {frameRecLog[logs[i].frame].width / 2, frameRecLog[logs[i].frame].height / 2}; // Set the origin to the center of the sprite  

            DrawTexturePro(
                spriteSheetLog, 
                frameRecLog[logs[i].frame], 
                (Rectangle){logs[i].x+1, logs[i].y+2, frameRecLog[logs[i].frame].width, frameRecLog[logs[i].frame].height},
                origin,
                0, 
                Fade(BLACK, 0.15f)
            ); 
            DrawTexturePro(
                spriteSheetLog, 
                frameRecLog[logs[i].frame], 
                (Rectangle){logs[i].x, logs[i].y, frameRecLog[logs[i].frame].width, frameRecLog[logs[i].frame].height},
                origin,
                0, 
                WHITE);
           
           //DrawCircleLines(logs[i].x , logs[i].y , frameRecLog[logs[i].frame].height/2, WHITE);
          
            //DrawText(TextFormat("speed: %.2f", logs[i].speed), 10, 50, 20, WHITE);  
            
        }
        
        
    }
    //DrawText(TextFormat("shit: %d", myCount), 10, 150, 20, WHITE);
}