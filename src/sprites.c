// sprites.c

#include "sprites.h"
#include <math.h>

// Include necessary declarations from main.c
extern Texture2D spriteSheetPlayer;
extern int frameWidthPlayer;
extern int frameHeightPlayer;
extern float frameCountPlayer;
extern Sound splashSfx;
extern Rectangle frameRecPlayer[];

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
    
    if (sprite->frame==1) {
        //PlaySound(splashSfx);
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
