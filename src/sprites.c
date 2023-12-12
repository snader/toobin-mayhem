// sprites.c

#include "sprites.h"

// Include necessary declarations from main.c
extern Texture2D spriteSheetPlayer;
extern int frameWidthPlayer;
extern int frameHeightPlayer;
extern float frameCountPlayer;
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

void DrawSprite(Sprite *sprite) {
    Rectangle sourceRec = {sprite->frame * frameWidthPlayer, 0, frameWidthPlayer, frameHeightPlayer};
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


void UpdateSprite(Sprite *sprite) {
    
    if (IsKeyDown(KEY_UP)) {
        sprite->speed += 0.04f;
    } else if (IsKeyDown(KEY_DOWN)) {
        sprite->speed -= 0.04f;
    }

    if (IsKeyDown(KEY_LEFT)) {
        sprite->degrees -= 1.0f;
    } else if (IsKeyDown(KEY_RIGHT)) {
        sprite->degrees += 1.0f;
    }
    
    if (sprite->degrees > 360) {
      sprite->degrees = 0.0f;
    } else if (sprite->degrees < 0) {
      sprite->degrees = 360.0f;
    }
    
    if (sprite->speed>1.0f) {
        sprite->speed=1.0f;
    }
    if (sprite->speed<-1.5f) {
        sprite->speed=-1.5f;
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
