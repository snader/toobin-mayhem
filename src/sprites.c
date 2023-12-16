// sprites.c

#include "sprites.h"
#include <math.h>

// Include necessary declarations from main.c
extern Texture2D spriteSheetPlayer;
extern Texture2D spriteSheetRipple;
extern int frameWidthPlayer;
extern int frameHeightPlayer;
extern float frameCountPlayer;
extern Sound splashSfxL;
extern Sound splashSfxR;
extern Rectangle frameRecPlayer[];
extern int DEAD;
extern int ALIVE;
extern Sprite* ripples;
extern int frameCount;
extern int myCount;

/*
void DrawEnemies(Sprite enemies[], size_t size) {
    for (size_t i = 0; i < size; ++i) {
        enemies[i].degrees++;
        
        if (enemies[i].isAlive == 1) {
            Rectangle sourceRec = {enemies[i].frame * frameWidthPlayer, 0, frameWidthPlayer, frameHeightPlayer};
            Vector2 origin = {frameWidthPlayer / 2, frameHeightPlayer / 2}; // Set the origin to the center of the sprite

            DrawTexturePro(
                spriteSheetPlayer,
                sourceRec,
                (Rectangle){enemies[i].x, enemies[i].y, frameWidthPlayer, frameHeightPlayer},
                origin,
                enemies[i].degrees,
                WHITE
            );
        }
    }
}
*/

void DrawPlayerSprite(Sprite *sprite) {
    
    int tempx, tempy;
    
    DrawRipples(ripples, sizeof(ripples));
    
    sprite->drawFrame = sprite->frame;
    if (IsKeyDown(KEY_LEFT)) {
        if (sprite->frame==1) {
            sprite->drawFrame = 3;
        }
        if (sprite->frame==2) {
            sprite->drawFrame = 4;
        }
    }
    if (IsKeyDown(KEY_RIGHT)) {
        if (sprite->frame==1) {
            sprite->drawFrame = 5;
        }
        if (sprite->frame==2) {
            sprite->drawFrame = 6;
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
        (Rectangle){sprite->x, sprite->y, frameWidthPlayer, frameHeightPlayer},
        origin,
        sprite->degrees,
        WHITE
    );
    
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
        
        // below player, tonen als y<0 en/of x<0
        
        //circlePos.x = sprite->x - sin(sprite->degrees * DEG2RAD) * 16;
        //circlePos.y = sprite->y + cos(sprite->degrees * DEG2RAD) * 16;
        // Draw the rotating circle below the sprite
        //DrawCircleV(circlePos, 2, WHITE);
}


void UpdatePlayerSprite(Sprite *sprite) {
    
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_DOWN)) {
        
        sprite->frameDelay++;
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
    
    if (!IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_UP) && !IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT)) {
        sprite->frame = 0;
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
        
    // Clamp the sprite within the screen bounds
    sprite->x = fmin(fmax(sprite->x, 0), GetScreenWidth() - 1);
    sprite->y = fmin(fmax(sprite->y, 0), GetScreenHeight() - 1); 
    
}

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

