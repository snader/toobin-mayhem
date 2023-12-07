// enemies.c

#include "enemies.h"

// Include necessary declarations from main.c
extern Texture2D spriteSheetPlayer;
extern int frameWidthPlayer;
extern int frameHeightPlayer;
extern float frameCountPlayer;
extern Rectangle frameRecPlayer[];


void DrawEnemies(Enemy enemies[], size_t size) {
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
